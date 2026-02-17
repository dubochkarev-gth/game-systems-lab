# Turn-Based Tactical Roguelike

Configuration-Driven, Focus-Centered Design Document (v4)

---

## Vision

A session-based, turn-based tactical roguelike where the primary gameplay lies in constructing a focused combat configuration in a Hub and validating it through deterministic encounters.

The game is not about mastering inputs.
It is about assembling a coherent system of trade-offs.

Combat exists to expose configuration weaknesses.

---

## Core Design Identity

This project is defined by three pillars:

### 1. Configuration Over Execution

The main strategic decisions happen before combat.

### 2. Focus as the Mechanical Core

Focus is the central tactical resource shaping combat rhythm.

### 3. Trade-Off Driven Progression

Every improvement introduces a weakness.

If a feature does not reinforce these pillars, it does not belong in the project.

---

## Core Gameplay Loop

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

The loop must remain stable.

---

## The Hub

The Hub is a configuration laboratory.

It is purely mechanical.

No narrative, no economy, no crafting.

### Hub Responsibilities

* Equip weapon
* Equip armor
* Select limited passive modifier
* Review previous run summary
* Start new run

The Hub must force meaningful choices.

---

## Configuration Model

A configuration consists of:

* Weapon modifier (offensive behavior change)
* Armor modifier (defensive behavior change)
* One passive effect
* Level-up specialization choices

There are no predefined classes.

Identity emerges from configuration coherence.

---

## Emergent Combat Roles (Additive Layer)

The game has no predefined roles or classes.
However, coherent configurations tend to express functional combat identities.

Roles are not selected.
They emerge from mechanical emphasis.

### Example Functional Roles

* **Pressure Holder**
  Focuses on survivability and threat manipulation.
  Generates Focus through defense and stabilizes pacing.

* **Burst Striker**
  Converts accumulated Focus into high-impact attacks.
  Fragile without defensive support.

* **Tempo Controller**
  Alters initiative, crit scaling, or enemy pacing.
  Changes combat rhythm rather than raw output.

* **Sustain Support**
  Improves stability through healing efficiency, mitigation, or Focus amplification.

These roles are not mandatory.

Enemy archetypes may stress specific axes, making certain configurations more effective in specific encounters.

Role identity is the result of trade-offs, not class selection.

---

## The Focus System (Core Mechanic)

Focus is the central resource of combat.

### Rules

* Generated primarily by defensive actions
* Consumed automatically by offensive actions
* Increases damage output
* Influenced by configuration

### Design Intent

Focus creates pacing tension:

Defensive play builds power.
Aggressive play spends power.
Mismanagement weakens output.

The question after a fight should be:

> Did I manage Focus well?

Not:

> Did I click fast enough?

---

## Combat System

### Philosophy

Combat is:

* Turn-based
* Initiative-driven
* Deterministic
* Fully logged in text

Combat tests configuration synergy.

---

### Core Actions

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

## Critical System (Focus-Scaled)

Critical hits are not flat RNG spikes.

They scale with Focus.

High Focus increases critical probability.

This makes crits partially predictable and tactical.

Randomness exists, but is influenced by player decisions.

---

## Initiative

* Recalculated each round
* Modifiable through configuration
* Can trade speed for survivability

Initiative must be readable and consistent.

Speed choices must introduce trade-offs.

---

## Trade-Off Driven Progression

Level-up choices are mutually exclusive and introduce tension.

Examples:

* +Attack, -Defense
* Faster initiative, lower max HP
* More Focus generation, weaker crit scaling
* Stronger block, slower turns

No pure stat growth.

Every choice narrows identity.

---

## Equipment Philosophy

Equipment changes behavior, not just numbers.

### Weapon Examples

* Burst Blade: consumes all Focus on attack
* Sustained Blade: consumes only 1 Focus per attack
* Risk Blade: higher crit scaling, lower base damage

### Armor Examples

* Guard Plate: block generates extra Focus
* Mirror Shell: reduces incoming crit chance
* Tempo Cloak: increases initiative, weakens block

No rarity tiers.
No loot inflation.

---

## Dungeon System

Dungeon is a structured sequence of mechanical tests.

### Structure

* 5–10 rooms
* First room safe
* Final room boss
* Logical node progression

No spatial map required for MVP.

---

### Room Types

* Combat
* Event
* Treasure
* Boss

Rooms determine encounter type only.

They do not override mechanical rules.

---

## Enemy Philosophy

Enemies exist to test specific configuration weaknesses.

Enemy archetypes stress different axes:

* High burst enemies test defensive builds
* Defensive enemies test sustained damage builds
* Fast enemies test initiative-focused builds
* Pressure enemies test Focus management

Enemy AI states remain:

* Aggressive
* Defensive
* Desperate

Health-threshold driven.

Enemy behavior must remain readable and deterministic.

---

## Inventory System

Inventory is minimal.

* One weapon
* One armor
* One passive
* Limited consumables

No micromanagement.
No stacking complexity.

---

## Narrative Layer

Narrative is optional and additive.

It must never:

* Override mechanics
* Introduce hidden rules
* Control outcomes invisibly

Text describes:

* Context
* Choices
* Results

Mechanics always remain transparent.

---

## MVP Scope

Included:

* Hub configuration layer
* Focus-centered combat
* Trade-off progression
* Dungeon sequence
* Text-based interface

Excluded:

* Meta progression
* Skill trees
* Complex build trees
* Save/load
* Crafting

---

## Design Anchor

When evaluating any feature, ask:

> Does this deepen configuration identity and reinforce Focus management?

If not, it does not belong.

---

## Intended Player Reflection

After a failed run, the player should think:

> My configuration lacked coherence.

or

> I mismanaged Focus.

If they think:

> The numbers were random.

The design has failed.
