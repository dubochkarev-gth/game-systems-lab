# Turn-Based Tactical Roguelike

<<<<<<< HEAD
## Configuration-Driven Combat Systems Prototype (v3)

---

# Vision

A session-based, turn-based tactical roguelike prototype where gameplay lies in constructing combat configurations in a Hub and validating them through deterministic encounters.
=======
Configuration-Driven, Focus-Centered Design Document (v4)

---

## Vision

A session-based, turn-based tactical roguelike where the primary gameplay lies in constructing a focused combat configuration in a Hub and validating it through deterministic encounters.
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

The game is not about mastering inputs.
It is about assembling a coherent system of trade-offs.

Combat exists to expose configuration weaknesses.

---

<<<<<<< HEAD
# Core Design Identity

This project is defined by three pillars:

## 1. Configuration Over Execution

The main strategic decisions happen before combat.

## 2. Mechanics Prototype Approach

Each run may emphasize one mechanic prototype while the broader combat framework remains stable.

## 3. Trade-Off Driven Progression
=======
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
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

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
<<<<<<< HEAD
```
=======
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

The loop must remain stable.

---

<<<<<<< HEAD
# The Hub
=======
## The Hub
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

The Hub is a configuration laboratory.

It is purely mechanical.

No narrative, no economy, no crafting.

<<<<<<< HEAD
## Hub Responsibilities
=======
### Hub Responsibilities
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

* Equip weapon
* Equip armor
* Select limited passive modifier
* Review previous run summary
* Start new run

The Hub must force meaningful choices.

---

<<<<<<< HEAD
# Configuration Model
=======
## Configuration Model
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

A configuration consists of:

* Weapon modifier (offensive behavior change)
* Armor modifier (defensive behavior change)
* One passive effect
* Level-up specialization choices

<<<<<<< HEAD
There are no classes.
=======
There are no predefined classes.
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

Identity emerges from configuration coherence.

---

<<<<<<< HEAD
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
=======
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
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

Combat is:

* Turn-based
* Initiative-driven
* Deterministic
* Fully logged in text

<<<<<<< HEAD
Combat tests configuration synergy and helps evaluate active mechanic prototypes.

---

## Core Actions
=======
Combat tests configuration synergy.

---

### Core Actions
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

* Attack
* Block
* Heal

Actions are universal.

Configuration modifies their behavior.

Examples:

* Block generates extra Focus
* Attack consumes all Focus for burst
* Heal consumes Focus instead of item
<<<<<<< HEAD
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

=======
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
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b
No loot inflation.

---

<<<<<<< HEAD
# Dungeon System

Dungeon is a structured sequence of tests.

## Structure
=======
## Dungeon System

Dungeon is a structured sequence of mechanical tests.

### Structure
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

* 5–10 rooms
* First room safe
* Final room boss
<<<<<<< HEAD

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
=======
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
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

* High burst enemies test defensive builds
* Defensive enemies test sustained damage builds
* Fast enemies test initiative-focused builds
<<<<<<< HEAD
=======
* Pressure enemies test Focus management
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

Enemy AI states remain:

* Aggressive
* Defensive
* Desperate

Health-threshold driven.

<<<<<<< HEAD
---

# Inventory System

Inventory is minimal.

* One weapon
* One armor
* One passive
* Limited consumables

No micromanagement.

=======
Enemy behavior must remain readable and deterministic.

---

## Inventory System

Inventory is minimal.

* One weapon
* One armor
* One passive
* Limited consumables

No micromanagement.
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b
No stacking complexity.

---

<<<<<<< HEAD
# Narrative Layer
=======
## Narrative Layer
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

Narrative is optional and additive.

It must never:

* Override mechanics
* Introduce hidden rules
* Control outcomes invisibly

Text describes:

* Context
* Choices
* Results

<<<<<<< HEAD
---

# MVP Scope
=======
Mechanics always remain transparent.

---

## MVP Scope
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

Included:

* Hub configuration layer
<<<<<<< HEAD
* Combat framework with one actively tested mechanic prototype (currently Focus)
* Trade-off level progression
=======
* Focus-centered combat
* Trade-off progression
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b
* Dungeon sequence
* Text-based interface

Excluded:

* Meta progression
* Skill trees
<<<<<<< HEAD
* Complex builds
=======
* Complex build trees
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b
* Save/load
* Crafting

---

<<<<<<< HEAD
# Design Anchor

When evaluating any feature, ask:

Does this deepen configuration identity and produce clear learnings for the current mechanic prototype?
=======
## Design Anchor

When evaluating any feature, ask:

> Does this deepen configuration identity and reinforce Focus management?
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

If not, it does not belong.

---

<<<<<<< HEAD
# Intended Player Reflection
=======
## Intended Player Reflection
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b

After a failed run, the player should think:

> My configuration lacked coherence.

or

<<<<<<< HEAD
> I understood how the current mechanic prototype affected my result.

If they think:

> The numbers were random

The design has failed.

---
=======
> I mismanaged Focus.

If they think:

> The numbers were random.

The design has failed.
>>>>>>> b8ff99151613b0f01665e495be3117b1ad40451b
