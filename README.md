## Overview

This project is an introduction to quantitative finance built around the Cox-Ross-Rubinstein (CRR) binomial model for pricing options. It combines the theoretical foundations of the model with an algorithmic implementation in C++, and aims to compare it with the Black-Scholes and Monte-Carlo approaches.

This is a personal project, carried out independently in my free time for my own learning. It was started in January 2026 and is **still in progress**: some sections are not yet written (see the status below), and the document may contain errors or inaccuracies. The full code is available on [GitHub](https://github.com/TomSaw31/Cox-Ross-Rubinstein).

## Topics

- Derivatives: forwards, futures, swaps and options (calls, puts, payoffs)
- Filtered probability spaces and self-financing portfolios
- No-arbitrage principle, replication and market completeness
- Fundamental theorems of asset pricing and risk-neutral measure
- Martingales and discounted prices
- Binomial trees and backward induction
- European and American options
- Recombining tree implementation in C++
- Brownian motion and Wiener process
- Itō integral and Itō's lemma
- Geometric Brownian motion and the Black-Scholes equation
- Option Greeks (Delta, Gamma, Vega, Theta)

## Theoretical Framework

The project starts from a short history of option pricing (Markowitz, Black-Scholes-Merton, Monte-Carlo, Cox-Ross-Rubinstein) and the basic vocabulary of derivatives, illustrated by a worked example of a call option.

It then builds the mathematical framework: filtered probability spaces, self-financing portfolios, the absence of arbitrage, replication strategies and complete markets. The fundamental theorems of asset pricing lead to the risk-neutral probability, under which discounted prices are martingales and the price of any derivative is the discounted expected value of its payoff.

## The Binomial Model

The model discretizes time into $N$ periods in which the underlying asset moves up by a factor $u$ or down by a factor $d$. Replicating the option with a portfolio of the risky and risk-free assets gives the risk-neutral probability $p = \frac{e^{r \Delta t} - d}{u - d}$ and the pricing formula $C_0 = e^{-r \Delta t}(pC_u + (1-p)C_d)$.

Because the tree is recombining, the number of nodes grows quadratically with the depth instead of exponentially. The price is computed by building the tree of underlying prices, evaluating the payoffs at maturity, then applying backward induction to the root. The same procedure extends to American options by comparing, at each node, the continuation value with the value of immediate exercise.

## Implementation

A first implementation in C++ uses two recombining trees of doubly-linked nodes, one for the prices of the underlying and one for the option values, which are traversed in parallel during the backward induction. It favors clarity over efficiency, and its execution time is measured as a function of tree depth, consistent with the expected quadratic complexity.

The simulations of Brownian motion (1D and 2D) used to illustrate the theory are written in R.

## Convergence to Black-Scholes

The project introduces the tools needed to link the discrete model to the continuous one: Brownian motion, Donsker's theorem, the Itô integral, Itô's lemma and the geometric Brownian motion, as well as the Black-Scholes partial differential equation and its assumptions.

## Current Status

This project is a work in progress. The following parts are still to be completed:

- Optimized implementation and performance analysis
- Derivation of the Black-Scholes formula and proof of the convergence of the CRR model
- Computation of the Greeks using the binomial model
- Comparison with Monte-Carlo simulations and trinomial trees
- Conclusion
