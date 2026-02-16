# Turn-Based Tactical Roguelike

## Configuration-Driven Combat Systems Prototype (v3)

---

# Vision

A session-based, turn-based tactical roguelike prototype where gameplay lies in constructing combat configurations in a Hub and validating them through deterministic encounters.

The game is not about mastering inputs.
It is about assembling a coherent system of trade-offs.

Combat exists to expose configuration weaknesses.

---

# Core Design Identity

This project is defined by three pillars:

## 1. Configuration Over Execution

The main strategic decisions happen before combat.

## 2. Mechanics Prototype Approach

Each run may emphasize one mechanic prototype while the broader combat framework remains stable.

## 3. Trade-Off Driven Progression

Every improvement introduces a weakness.

If a feature does not reinforce these pillars, it does not belong in the project.

---

# Core Gameplay Loop

```
Main Menu
↓
Enter Hub
↓
Select Configuration (equipment + modifiers)
↓
Generate Dungeon
↓
Room → Resolve Encounter
↓
Evaluate Outcome
↓
Return to Hub
↓
Adjust Configuration
```

The loop must remain stable.

---

# The Hub

The Hub is a configuration laboratory.

It is purely mechanical.

No narrative, no economy, no crafting.

## Hub Responsibilities

* Equip weapon
* Equip armor
* Select limited passive modifier
* Review previous run summary
* Start new run

The Hub must force meaningful choices.

---

# Configuration Model

A configuration consists of:

* Weapon modifier (offensive behavior change)
* Armor modifier (defensive behavior change)
* One passive effect
* Level-up specialization choices

There are no classes.

Identity emerges from configuration coherence.

---

# Focus Mechanic Prototype (Current Iteration)

Focus is currently a prototype mechanic being tested inside the combat framework.

### Rules

* Generated primarily by defensive actions
* Consumed automatically by offensive actions
* Increases damage output
* Can be modified by configuration

### Design Intent

Focus is intended to create pacing tension during this prototype phase:

* Defensive play builds power
* Aggressive play spends power
* Mismanagement weakens output

A key test question after a fight is:

> Did Focus meaningfully influence my tactical choices?

Not:

> Did I click fast enough?

---

# Combat System

## Philosophy

Combat is:

* Turn-based
* Initiative-driven
* Deterministic
* Fully logged in text

Combat tests configuration synergy and helps evaluate active mechanic prototypes.

---

## Core Actions

* Attack
* Block
* Heal

Actions are universal.

Configuration modifies their behavior.

Examples:

* Block generates extra Focus
* Attack consumes all Focus for burst
* Heal consumes Focus instead of item
* Critical chance may scale with Focus in this prototype

---

## Critical System (Updated)

In the current Focus prototype, critical hits are no longer flat RNG spikes.

They may scale with Focus.

High Focus can increase critical probability.

This makes crit behavior partially predictable for prototype evaluation.

---

## Initiative

* Recalculated each round
* Modifiable through configuration
* Can trade speed for survivability

Initiative must be readable and consistent.

---

# Trade-Off Driven Progression

Level-up choices are mutually exclusive and introduce tension.

Examples:

* +Attack, -Defense
* Faster initiative, lower max HP
* More Focus generation, weaker crit scaling (prototype option)
* Stronger block, slower turns

No pure stat growth.

Every choice narrows identity.

---

# Equipment Philosophy

Equipment changes behavior, not just numbers.

Examples:

### Weapon Types

* Burst Blade: consumes all Focus on attack (prototype behavior)
* Sustained Blade: consumes only 1 Focus per attack (prototype behavior)
* Risk Blade: higher crit scaling, lower base damage (prototype behavior)

### Armor Types

* Guard Plate: block generates extra Focus (prototype behavior)
* Mirror Shell: reduces incoming crit chance
* Tempo Cloak: increases initiative, weakens block

No rarity tiers.

No loot inflation.

---

# Dungeon System

Dungeon is a structured sequence of tests.

## Structure

* 5–10 rooms
* First room safe
* Final room boss

Rooms are logical nodes.

No spatial map in MVP.

---

## Room Types

* Combat
* Event
* Treasure
* Boss

Rooms determine encounter type only.

---

# Enemy Philosophy

Enemies exist to test specific weaknesses.

Enemy archetypes should stress different axes:

* High burst enemies test defensive builds
* Defensive enemies test sustained damage builds
* Fast enemies test initiative-focused builds

Enemy AI states remain:

* Aggressive
* Defensive
* Desperate

Health-threshold driven.

---

# Inventory System

Inventory is minimal.

* One weapon
* One armor
* One passive
* Limited consumables

No micromanagement.

No stacking complexity.

---

# Narrative Layer

Narrative is optional and additive.

It must never:

* Override mechanics
* Introduce hidden rules
* Control outcomes invisibly

Text describes:

* Context
* Choices
* Results

---

# MVP Scope

Included:

* Hub configuration layer
* Combat framework with one actively tested mechanic prototype (currently Focus)
* Trade-off level progression
* Dungeon sequence
* Text-based interface

Excluded:

* Meta progression
* Skill trees
* Complex builds
* Save/load
* Crafting

---

# Design Anchor

When evaluating any feature, ask:

Does this deepen configuration identity and produce clear learnings for the current mechanic prototype?

If not, it does not belong.

---

# Intended Player Reflection

After a failed run, the player should think:

> My configuration lacked coherence.

or

> I understood how the current mechanic prototype affected my result.

If they think:

> The numbers were random

The design has failed.

---
