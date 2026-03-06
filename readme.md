# Connect4-MCTS-AI (Competitive Agent)
> **Course Project:** Tsinghua University "Introduction to Artificial Intelligence"  


This repository contains a high-performance **Connect4 AI Agent** implemented in **C++**. The agent is designed to handle advanced variations of the game, including **randomized board dimensions** (9-12 rows/cols) and **randomly placed obstacles** (unplayable slots).

## 🧠 Core Algorithm: UCT (MCTS)
The agent utilizes the **Monte Carlo Tree Search (MCTS)** framework, specifically the **UCT (Upper Confidence Bound applied to Trees)** algorithm:
* **Standard Pipeline:** Fully implements `treePolicy` (Selection & Expansion), `defaultPolicy` (Simulation), and `backUP` (Backpropagation).
* **Profit Function:** Employs a refined reward system ($profit$) based on game outcomes to guide the search process.
* **Parameter Tuning:** Through extensive experimentation, the exploration constant ($C$) was optimized to **0.7**, achieving a superior balance between exploitation of known paths and exploration of new strategies.

## ⚙️ High-Performance Engineering (C++)
To maximize simulations within strict time constraints, several low-level optimizations were implemented:
* **Static Node Pool:** Instead of using slow dynamic memory allocation (`new`/`delete`), a `static Node* node_pool[]` was implemented to manage memory. This significantly reduces overhead and boosts search frequency.
* **Architecture:** The design follows modular principles for efficient state management and rapid win-condition evaluation.

## 🎯 Advanced Competitive Strategies
Beyond the standard MCTS, the agent incorporates heuristic pruning and strategic refinements to ensure robustness:
* **Immediate Move Detection:** Pre-search checks for **"Must-Win"** and **"Must-Block"** points. Priority is set to: $Winning > Blocking$.
* **Anti-Trap Logic:** Detects and prunes "Giving-Win" moves—positions that would grant the opponent an immediate victory—saving computational resources.
* **Fast-Win & Slow-Loss:** 
    * **Fast-Win:** If multiple winning paths are detected, the agent selects the shortest depth to terminate the game quickly.
    * **Slow-Loss:** In inevitable losing scenarios, it selects the longest path to prolong the game, maximizing the chance for the opponent to make a mistake.