# Turn-Based Tactical Roguelike

## Configuration-Driven, Focus-Centered Design Document (v3)

---

# Vision

A session-based, turn-based tactical roguelike where the primary gameplay lies in constructing a focused combat configuration in a Hub and validating it through deterministic encounters.

The game is not about mastering inputs.
It is about assembling a coherent system of trade-offs.

Combat exists to expose configuration weaknesses.

---

# Core Design Identity

This project is defined by three pillars:

## 1. Configuration Over Execution

The main strategic decisions happen before combat.

## 2. Focus as the Mechanical Core

Focus is the central tactical resource shaping combat rhythm.

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

# The Focus System (Core Mechanic)

Focus is the central resource of combat.

### Rules

* Generated primarily by defensive actions
* Consumed automatically by offensive actions
* Increases damage output
* Can be modified by configuration

### Design Intent

Focus creates pacing tension:

* Defensive play builds power
* Aggressive play spends power
* Mismanagement weakens output

The question after a fight should be:

> Did I manage Focus well?

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

Combat tests configuration synergy.

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
* Critical chance scales with Focus

---

## Critical System (Updated)

Critical hits are no longer flat RNG spikes.

They scale with Focus.

High Focus increases critical probability.

This makes crits partially predictable and tactical.

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
* More Focus generation, weaker crit scaling
* Stronger block, slower turns

No pure stat growth.

Every choice narrows identity.

---

# Equipment Philosophy

Equipment changes behavior, not just numbers.

Examples:

### Weapon Types

* Burst Blade: consumes all Focus on attack
* Sustained Blade: consumes only 1 Focus per attack
* Risk Blade: higher crit scaling, lower base damage

### Armor Types

* Guard Plate: block generates extra Focus
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
* Focus-centered combat
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

Does this deepen configuration identity and reinforce Focus management?

If not, it does not belong.

---

# Intended Player Reflection

After a failed run, the player should think:

> My configuration lacked coherence.

or

> I mismanaged Focus.

If they think:

> The numbers were random

The design has failed.

---
