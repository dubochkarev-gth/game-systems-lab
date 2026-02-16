Turn-Based Tactical Roguelike
Configuration-Driven, Text-Based Design Document (v2)
Vision

A session-based, turn-based tactical roguelike where the primary gameplay lies in assembling and refining a functional configuration in a Hub, then validating that configuration through deterministic combat encounters.

The project emphasizes:

Mechanics-first design

Tactical clarity and readability

Configuration as strategic gameplay

Combat as validation, not spectacle

Text as interface, not decoration

Expandability through additive systems

Narrative is a flexible layer that enhances context but never controls mechanics.

Core Design Pillars

Configuration Is the Real Gameplay
Strategic decisions are made in the Hub before entering the dungeon.

Combat Is Validation
Encounters test the coherence of the configuration rather than overwhelm the player with complexity.

Power Emerges from Trade-offs
Strength comes from specialization and synergy, not raw stat growth.

Deterministic & Readable Systems
All mechanics are transparent and understandable through text alone.

Expandable by Addition
New features must layer on top of the existing structure without requiring system rewrites.

Core Gameplay Loop
Main Menu
↓
Enter Hub
↓
Configure Loadout / Select Modifiers
↓
Generate Dungeon
↓
Enter Room
↓
Resolve Encounter (Combat / Event / Loot)
↓
Return to Hub (Adjust Configuration)
↓
Next Run


The loop must remain intact.
All new systems must reinforce this structure.

Game States

The game operates through clearly separated logical states:

Main Menu

Hub

Dungeon Exploration

Combat

Event / Narrative Choice

Inventory Interaction

Level Up

Game Over

Each state owns its logic.
The UI only reflects the current state.

Hub System

The Hub is a strategic planning space.

It is not a town, narrative location, or social system.
It is a configuration laboratory.

Hub Responsibilities

Equip weapon and armor

Select limited modifiers

Choose specialization upgrades

Review previous run results

Prepare next dungeon run

The Hub must remain:

Minimal

Text-driven

Mechanically focused

Configuration Philosophy

A configuration consists of:

Equipment (weapon, armor)

Passive modifiers (relic-style effects)

Limited specialization choices

Tactical orientation (offense/defense balance)

Configurations must:

Contain meaningful trade-offs

Encourage specialization

Avoid universal dominance

There are no complex build trees in MVP.

Dungeon System
Structure

5–10 rooms per run

First room is safe

Final room is a boss

Controlled randomness

Rooms are logical nodes, not spatial grids.

Room Types

Combat

Event

Treasure

Boss

Rooms do not contain logic.
They only determine encounter type.

Player Model

The player is defined by:

Health

Offensive capability

Defensive capability

Initiative

Experience and Level

There are no classes in MVP.

Identity emerges from configuration, not predefined archetypes.

Progression System
Experience Sources

Combat encounters

Narrative events

Level-Up Philosophy

Level-ups present mutually exclusive choices that reinforce specialization.

Examples:

Increase attack but reduce defense

Improve Focus generation but lower initiative

Safer blocks but weaker critical potential

Progression must create divergence between runs.

Flat stat growth without trade-offs is discouraged.

Inventory System
Philosophy

Inventory exists to modify configuration decisions, not to create management overhead.

Small and readable

No sorting systems

No weight systems

No rarity tiers in MVP

Equipment Rules

One weapon equipped

One armor equipped

Limited passive modifiers

Consumables removed on use

Items should modify behavior rather than simply increase numbers.

Examples:

Block generates additional Focus

Critical chance scales with Focus

Healing consumes Focus instead of item

Combat System
Philosophy

Combat is:

Turn-based

Initiative-driven

Fully text-logged

Deterministic and readable

Combat validates configuration choices.
It must not rely on visual spectacle.

Core Actions

Attack

Block

Heal

Each action produces a clear textual result.

Focus System

Focus is a temporary tactical resource.

Generated through defensive actions

Automatically consumed by offensive actions

Grants damage bonus

Encourages pacing and planning

Focus reinforces deliberate play rather than button-spamming.

Critical Hits

Flat chance system

Damage multiplier

Explicitly logged

Initiative

Determines turn order

Recalculated each round

Simple and predictable

Enemy Behavior

Enemies operate using internal states:

Aggressive

Defensive

Desperate

State transitions are health-threshold based.

Enemy design must reinforce configuration testing.

Narrative & Text Layer

Narrative is:

Contextual

Flexible

Expandable

Narrative never overrides mechanical systems.

Text is responsible for:

Describing context

Presenting numbered choices

Communicating outcomes

Narrative depth is secondary to mechanical clarity.

UI Philosophy

The UI is purely text-driven.

Responsibilities:

Display descriptions

Display numbered choices

Handle player input

Display combat logs

No windows, overlays, or complex layouts.

MVP Scope Included

Hub configuration layer

Dungeon generation

Turn-based combat

Inventory system

Leveling with trade-offs

Text-based interface

Explicitly Excluded

Saving/loading

Meta-progression

Skill trees

Complex builds

World map

Crafting systems

Procedural stat scaling

Planned Extensions

Future layers may include:

External data-driven content (JSON)

Status effects

Equipment synergies

Conditional narrative events

Skill checks

These must be added without breaking the core loop.

Design Anchor

When adding any new feature, always ask:

Does this strengthen configuration as strategic gameplay and combat as validation?

If the answer is no, the feature does not belong in this project.
