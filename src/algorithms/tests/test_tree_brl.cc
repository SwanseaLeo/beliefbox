/* -*- Mode: C++; -*- */
// copyright (c) 2006-2017 by Christos Dimitrakakis <christos.dimitrakakis@gmail.com>
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifdef MAKE_MAIN

/// Other things
#include "MersenneTwister.h"


/// Bayesian RL includes
#include "DiscreteMDPCounts.h"

/// The main thing to test
#include "TreeBRL.h"

/// The basic environment
#include "DiscreteChain.h"
#include "Blackjack.h"
#include "InventoryManagement.h"

/// STD
#include <iostream>
#include <memory>
using namespace std;

int main(void) {
    RandomNumberGenerator* rng;
    MersenneTwisterRNG mersenne_twister;
    rng = (RandomNumberGenerator*) &mersenne_twister;
    rng->seed();
    int n_states = 5;
    int n_actions = 2;
    int max_planning_horizon = 3;
    real discounting = 0.9;
    int n_samples = 8; ///< number of state samples when branching
    int n_mdp_samples = 128; ///< number of MDP samples at leaf nodes

    printf("# Making environment\n");
    unique_ptr<DiscreteEnvironment> environment;
    environment = make_unique<DiscreteChain>(n_states);
    //environment = make_unique<Blackjack>(rng);
    //environment = make_unique<InventoryManagement>(10, 5, 0.2, 0.1);
    n_states = environment->getNStates();
    n_actions = environment->getNActions();

    printf("# Setting up belief\n");
    Matrix rewards(n_states, n_actions);
    for (int s=0; s<n_states; ++s) {
        for (int a=0; a<n_actions; ++a) {
            rewards(s,a) = environment->getExpectedReward(s, a);
        }
    }
    DiscreteMDPCounts belief(n_states, n_actions);

    // simplify things by fixing the rewards
    belief.setFixedRewards(rewards);


    printf("# full sampling\n");
    for (int planning_horizon=0;
         planning_horizon<max_planning_horizon;
         planning_horizon++) {
        TreeBRL tree (n_states, n_actions, discounting, &belief, rng, planning_horizon);
        // Set state to 0
        tree.Reset(0);

        int state = environment->getState();
        int reward = environment->getReward();
                tree.Act(reward, state);
        
        // Calculate a belief tree
        tree.CalculateBeliefTree(n_mdp_samples);
    }

    printf("# sparse sampling\n");
    for (int planning_horizon=0;
         planning_horizon<max_planning_horizon;
         planning_horizon++) {
        TreeBRL tree (n_states, n_actions, discounting, &belief, rng, planning_horizon);
		
        // Set state to 0
        tree.Reset(0);
		
        // Calculate a belief tree
        tree.CalculateSparseBeliefTree(n_samples, n_mdp_samples);
    }

    return 0;
}

#endif