# Game Design Document
# Overview
“Project F” is a RTS game with survival and exploration elements. The main idea behind the design of this project is to make the game as accessible as possible, avoiding all the mechanics that add unnecessary complexity to the gameplay (such as extensive micromanagement).

The game will be campaign based with a gameplay loop that lasts about 30~45min and, gameplay wise, players will be able to manage and control different units and buildings as well as gather different resources.

The main objective of the game will be to clear all four different zones of the map that surround the players' headquarters while trying not to be run over by the periodic enemy raids.


# Plot
We are in the 80s, a time where journalism is one of the main pillars of information. Our character, Vrainen Heber, is a journalist who works for the Austrian national newspaper and is on the verge of dismissal. His continuous failures in the company when publishing inconspicuous, ridiculous and even, in some cases, false articles, have led him to the serious situation of seeing himself as a homeless person in a matter of weeks.

Our character is on a critical situation but still has something to hold on to. The newspaper editor gives him one last chance and suggests to him to write an article about a possible case of corruption and data retention in the Austrian government that directly concerns the actual President: Rudolf Kirchschläger.

Motivated by this last opportunity, he is determined to uncover the government once and for all. But how would he do it? Who was him to deal with a first priority subject when his most important article had been about a robbery in a greengrocer? he didn't have the means or the resources to get such type of information. At least not legally.

Vrainen took out the little money he had been saving during these bad months, since he barely had enough day to day, all he had were 500 shillings. 
These past weeks, in the streets a name was being rumored, a certain scientist of bad reputation, with macabre ideas but with a magnificent intellect, who had just moved to the city after being expelled from the engineering university in Berlin. In a few days he was able to find him, Dr. Luka Ackermann.

Vrainen approached the Professor and explained his proposal, Dr. Luka agreed without a doubt. Since the doctor was a nanotechnology expert, he told him about a method that he had been working, it remained a secret for years and was the reason why he was expelled from the university. 

Everything consists on doing a “nano-transformation”, a concept invented by the professor himself, where his molecules will be converted into bits and introduced into the power supply of the professor's computer. This way, Vrainen will be able to be within the network and use everything at his hand such as electricity flows, data structures, servers or wi-fi networks, among others, to get to the bottom of the government's data system and uncover them, keeping his job and making a name for himself in the journalistic world.

3,2,1 ... Immersion.


# Game Pillars
- **Approachable:** The projects' main goal is to be as accessible as possible, avoiding any mechanics that add unnecessary complexity to the gameplay and simplifying the core concepts that comprise the RTS genre.

- **Exploration:** Exploring the map grants players different benefits such as resources and information on the location of enemy bases. Said benefits should encourage players to avoid staying at their base for extended periods of time. 

- **Survival:** There will be periodical enemy raids that should force players to balance exploring with defending. Players should feel the need to pay attention to the state of their base and not leave it unguarded for long.


# Game Flow
- {Control mapping}


# Gameplay
## Objectives
The main objective of the game will be to conquer all 4 security servers and get access to the government's classified information.

- **1. Short-term Objectives:** Explore, gather resources and defend the Headquarters.
- **2. Mid-term Objectives:** Construct Buildings, expand the army and attack enemy outposts. 
- **3. Long-term Objectives:** Complete all quests and eliminate all enemies.

To achieve the aforementioned objectives, players will need to correctly manage their units and resources while exploring the map and defending their Headquarters. A minimap will be implemented to aid them in navigating the environment and keeping track of their immediate objectives. 


## Win/Loss Conditions
### Win Condition
Only by completing all four quests can players reach the win condition. The quests are the following:
- **Quest 1: ---**
- **Quest 2: ---**
- **Quest 3: ---**
- **Quest 4: ---**

The difficulty of each quest will depend on which quests players complete first, so the difficulty will be divided in 4 Tiers. If additional information on this topic is required, see the **Enemy Outposts** subsection in the **Map** section and the **Rewards** section.

### Loss Condition
There will be 2 loss conditions:
- **No home to return to.** The player's Headquarters have been destroyed.
- **No units remaining.** The player has lost all units and cannot produce more (either resource wise and infrastructure wise)

## Gameplay Systems
The gameplay of this project will be based around resource, unit and building management,exploration, and survival.

### Exploration and objectives 
Four distinguished zones, which will surround the player's Headquarters, will be available for exploration. Each one of them will have their own main quest to complete. Moreover, in each of the areas resources and isolated patrolling group of enemies could be found. Exploring and exposing the enemy bases will be highly rewarded.

Resources, exploration wise, refer to locations from where resources will be able to be extracted. {?}

### Resources and resource gathering
There will be 3 different resources, each with their own particular use and acquisition conditions:

- **Data:** Dedicated to creating buildings. {Talk about quantity, spread over the map and acquisition conditions}
- **Electricity:** Dedicated to spawning units. {Talk about quantity, spread over the map and acquisition conditions. Maybe only a set amount can be acquired by building "memory centers" or something like that. It would work wonders with unit management. Just an idea though.}
- **Bytes:** Dedicated to upgrading. There will be a highly limited amount of it spread all over the map and it will require accurate management, regarding which building and which state of the game the player makes use of it.

{While some buildings are specifically made for that purpose, exploring will have its own advantages, like finding "bytes" the upgrading resource which won't have an spececific building to exploit it.}{?}
The remaining resources can be both obtained by exploring and gathered with the given building.

{I cannot see buildings that constantly extract resources working well with a gathering unit, either we have a passive income from buildings or we have resource extraction points(mines). To have both all the resources should be designed around that}

### Buildings
The building system will be divided in three categories: 
- **Ally Buildings:** Buildings available to the player.
- **Enemy Buildings:** Buildings available to the enemies.
- **Neutral Structures:** Structures that will be spread all over the map. Will serve a decorational/environmental purpose and are not destructible. 

The ally buildings will be further classified into 2 more categories: 
- **Production Buildings:** For unit and resource production. 
- **Defense Buildings:** For purely defense purpouses. 

All demolishable buildings can be in the 3 following states:
- **The building has sustained no damage:** Will be fully functional.
- **The building has sustained some damage:** Will repair itself over time. Some buildings will stop working (Barracks...)
- **The building has been destroyed:** Can only be reparied by a {Insert Unit Name Here} along with the spending of some amount of resources.

It will be possible to upgrade buildings, should the required resource in the necessary amounts be available.

### Unit Creation & Management
In every new game, players will start with a single scout and a single gatherer. From here on out, it is the player's job to gather resources and start building new structures along with the recruitment of new troops. 

If a unit is left-clicked on, a menu will emerge providing information on all the actions that are available to the unit. Furthermore, shortcuts will be provided to organize and manage all the units more efficiently.

All units will have shortcuts to do some actions however some of them require exclusive shortcuts due to their unique available actions. {Unique unit actions? Like which?}

All the possible unit actions will be listed down below:
- **Movement:** All units can move. To move a unit, it should be selected first by right-clicking on it and then left-clicking at the destination. To select multiple units, right-click and drag the mouse over all the units that are to be moved. A rectangle will appear and every unit inside it's area will be selected.

- **Combat:** All units (except Gatherers) can attack enemy units. 

  - **While Idle:** If an enemy unit enters the range of a unit, said unit will attack it if there was no order previously given to the aforementioned, should the enemy unit get out of range, the ally unit will follow it until it is destroyed. {Idle it's necesary??}

  - **When given a target:** To attack a specific enemy unit, any selected unit can be given a target by left-clicking on an enemy. Should the enemy unit be out of range, the selected units will path towards that enemy.
  
  - **While in Guard Mode:** If a unit is in Guard Mode, it will remain static at the given position. The unit will follow the same behaviour as while idle, with the only difference being that if the enemy gets out of range, it will not be followed after.
  
- **Repairs:** Only Gatherers can perform this action. Should a building be completely destroyed, only a Gatherer can restore it back to a fully functional state. By right clicking on an Idle Gatherer and left-clicking on a destroyed building, the gatherer will path towards the building and start the repairs. Gatherers can also be sent to speed up the natural recovery rate of damaged buildings.

{Follow the same structure as in the Buildings subsection, if possible}

# Buildings, Units & Resources
## Buildings
- **Wall**
- **Recollector**
- **Defensive**
- **Barracks**
- **Hall**

### Ally Buildings
- **Headquarters:** The players' center of operation. Should this building be destroyed, the game would be over.
- **Production Buildings:** Buildings that can produce either resources or units.
  - **Garrison:**
  - **Generator:** {Example}

- **Defense Buildings**
  - **Firewall:**
  - **Pitfall:** {Example}

### Enemy Buildings
- **Headquarters:**
- **Defense Buildings**
  - **Firewall:** 

### Neutral Structures
- **Rock:**
- **Empty Building:**
{Examples}

### Technology Tree
{Will our game have a technology tree? At least represent the building upgrades}

## Units
### Allies
There are 4 types of units that can be recruited, all of which different types of spaceships:

- **Scout**:  The unit with the fastest movement speed. It has the lowest HP and damage. Useful to quickly scout the map.
- **Recollector**: The only unit that can collect resources. It has low movement speed and low HP. 
- **Infantry**: Main combat unit. Performs single-target attacks. It has average Movement Speed, average HP, and average damage. 
- **Heavy**: Special combat unit. Performs AoE attacks. It has low Movement Speed, high HP, High damage but slow attack speed.

{Make a table with images of the sprites of each unit as well as the unit name, HP, Cost, Movement Speed, Range, Damage and Attack Speed.}

| Unit        | Sprite       |  HP    | Movement Speed  | Range | Damage | Attack Speed | Cost |
| ----------- |:------------:| ------:|:-------------:|:-----:|:------:|:----------:|:----:|
|**Scout**     | ![alt text](https://github.com/BarcinoLechiguino/Project-RTS/blob/master/Docs/Images/ArtBible/Units/scout.png?raw=true) | ?? |?? |?? |?? |?? |?? |
| **Recollector** | ![alt text](https://github.com/BarcinoLechiguino/Project-RTS/blob/master/Docs/Images/ArtBible/Units/collector.jpg?raw=true)   | ??   |?? |?? |?? |?? |?? |
| **Infantry** |  ![alt text](https://github.com/BarcinoLechiguino/Project-RTS/blob/master/Docs/Images/ArtBible/Units/infantry.png?raw=true)  |  ??   |?? |?? |?? |?? |?? |
| **Heavy** | ![alt text](https://github.com/BarcinoLechiguino/Project-RTS/blob/master/Docs/Images/ArtBible/Units/heavy.png?raw=true)    |   ??  |?? |?? |?? |?? |?? |

### Enemies
They will have the same structure, but the enemy won’t have collectors and explorers as their objective is just to destroy your base. 

- **Infantry**
- **Heavy**
- **Final Boss**

{Make a table with images of the sprites of each unit as well as the unit name, HP, Cost, Movement Speed, Range, Damage and Attack Speed.}

| Enemy       | Sprite       |  HP    | Movement Speed  | Range | Damage | Attack Speed | Cost |
| ----------- |:------------:| ------:|:-------------:|:-----:|:------:|:----------:|:----:|
| **Infantry** |  ![alt text]()  |  ??   |?? |?? |?? |?? |?? |
| **Heavy** | ![alt text]()    |   ??  |?? |?? |?? |?? |?? |
| **Final Boss** | ![alt text]()    |   ??  |?? |?? |?? |?? |?? |

### Resources 
- **For building**: chunks of data.
- **For recruiting**: electricity.
- **For upgrading**: bytes.


### Balancing
#### Unit Balancing

{See the payoffs table at the Devcrumbs GDD}

#### Resource Balancing


This will be a variant changing while we develop the game trying to adjust it to be as fair as possible with the player, without leaving aside the difficulty as a core aspect of it, taking into account our aim for the game and also our limitations.
At this current state of the development we can’t provide with any exact information of the exact balance numbers we will be working on, like amount of resources needed to build an structure, the time to train or create a unit, etc.


# Map

{Here all the zones of the map as well as all the elements that are to be placed in said zones should be mentioned.}
{Additionally, mechanics such as the Fog of War should be explained in full detail.}

{Add a PNG showing all the map in its full extension}

## Player Headquarters/Server/Whatever
The players' base will remain the same through all playthroughs. It will be the main center of operations and all buildings will only be possible to create within a certain range around.

{PNG of the Player's Base}

{Explain the different functions of the base}

The Headquarters will be occasionally raided by the enemy, so effective defenses must be built around it. Moreover, there should always be some troops stationed there, in case the exploring military force cannot make it back in time.

## Enemy Outposts
There will be a total of 4 enemy outposts, one on each zone. All posts will be equipped with defensive buildings and their own military task force.

{PNGs of all 4 outposts. Should each have a different design? Should each be the same but with different colours?}

Each destroyed outpost increases the defenses of the remaining ones, so if at the beginning al 4 outposts have X units and X walls, once one has been run over, the remaining ones will have X+5 units and X+10 walls, for instance.

### Tier 1 Outpost
- **North Outpost:**
- **South Outpost:**
- **East Outpost:**
- **West Outpost:**

### Tier 2 Outpost
- **North Outpost:**
- **South Outpost:**
- **East Outpost:**
- **West Outpost:**

### Tier 3 Outpost
- **North Outpost:**
- **South Outpost:**
- **East Outpost:**
- **West Outpost:**

### Tier 4 Outpost
- **North Outpost:**
- **South Outpost:**
- **East Outpost:**
- **West Outpost:**

{Maybe making 4 tables, one for each tier, will make things clearer}

{PNGs of the same outposts with increased difficulty}

## Neutral Areas
### Resources to be found

### Enemy Patrols


## Fog of War
The fog of war will cover everything on the map except the players' headquarters. It will have 3 distinct states:

- **Black:** Unexplored areas will be covered by a black tile.
- **Semi-Transparent:** Areas which have already been explored but are no longer within the players' range of sight (No ally buildings or units near that area). The terrain layout of such areas can still be seen, but any changes such as new enemy buildings or units will not be revealed until said area is within range again. These areas will be covered by a semi-transparent black tile.
- **Visible:** Areas that have been already explored and ally entities are currently within. These areas will not be covered.

{PNGs of the different stages of the Fog of War}

The Fog of War will also be represented on the minimap.

## Minimap
The minimap will be placced at the top-”..” corner of the screen. It will offer a view on a section of the map as well as highlighting both ally (in green/blue) and enemy (in red and if they are within sight) units and buildings.

As the map may be too big, the minimap’s radius will be limited and the minimap will move within the map. Next to the minimap we have a button that, by pressing it, will allow players to change the minimap range.

The camera will be able to be manipulated by clicking and dragging on the minimap (the camera will be centered around wherever the mouse cursor is at).


# Learn To Play
At the start of every game, an icon with the face of the Dr. Ackermann will appear at the top-”...” corner of the screen. He will welcome the players to the simulation will convey to them the importance of the mission they are taking part in. 

The Doctor will show you the controls of the game as well as explain and the main mechanics of the game (Resource Gathering, Combat...)


# Rewards
Each of the 4 main quests will reward players with additional resources {?}. As the difficulty for each quest will vary depending on which route player's take, all rewards have been separated in tiers:

## Tier 1 Rewards
- **Mission 1:**
- **Mission 2:**
- **Mission 3:**
- **Mission 4:**


## Tier 2 Rewards
- **Mission 1:**
- **Mission 2:**
- **Mission 3:**
- **Mission 4:**

## Tier 3 Rewards
- **Mission 1:**
- **Mission 2:**
- **Mission 3:**
- **Mission 4:**

## Tier 4 Rewards
- **Mission 1:**
- **Mission 2:**
- **Mission 3:**
- **Mission 4:**

{Maybe making 4 tables, one for each tier, will make things clearer}


# Controls
We recommend to play the game with both mouse and keyboard, but it can be played also with gamepad.
Although it is heavily advise to play with mouse and keyboard, the game can also be played with a game pad:

## Mouse and Keyboard Control Map
### Mouse: 
- Right click: Target an enemy/resource/building
- Left click: Select a building/unit
- Left drag: Select a group of units

### Keyboard:
- 

{Add a PNG with a visual representation of the map}

## Gamepad Control Map

{Add a PNG with a visual representation of the map}


## { -----------------------------------------------------}


# Previous Text {Erase Later}
### Overview
{The main objective is to explore the map through a fog keeping the player from having a complete vision, fortify your base in order to hold the seizing and gather resources across the map to beat the final boss that will be waiting for you at the end of it. Summarizing, players will have to spawn and manage different troops and buildings while balancing exploring the map and defending their Headquarters. 
{Revise, incorrect structure.}

The game offers unique and challenging experiences where the player will have no other option than be forced to make use of their dexterity as well as put their five senses in disposal in order to success on his mission without leaving the nerves get over him. {Revise, incorrect structure and expressions}

You will have to explore all the surroundings, find resources, complete quests to gain more and more materials to build the strongest wing and defeat the final boss. Anyway, don’t leave alone your city, don’t go too far away as hordes of enemies will appear to besiege your base. Think strategies and make sure to cover your back.}
{Revise, incorrect structure and format not formal enough.}

### Lore

### Game Pillars
**Exploration:** Exploration benefits will be balanced with the periodic enemy raids, as no matter how many resources and information is at hand, an empty base will not defend itself. The more the map is explored, the better units and buildings that the player can make.

**Horde System:**  Horde after horde enemies will keep coming with a break in between so you can prepare yourself against the next seizing or even explore and complete quests. The time set between hordes it is thought to give the player enough time to engage on a new quest, without taking an eye out of the timer and the base. The difficulty will grow within the hordes.
{Revise. Many grammar mistakes and the structure is all over the place. The 5 questions (What, when...) are not answered clearly}

**Combat system:** By clicking and dragging you can select the amount of units you want to move wherever the player wants as long as it's a walkable area for the units. When enemy units slip into the attack area of those units, they will begin to attack them. This system improves the strategy side of the game, placing your units to defend the weaker side of your base.
{Revise. Incorrect structure. Weak reasoning behind the decisions made on the combat system}

### Game Flow

## Gameplay

### Gameplay Systems
#### Exploration and objectives 
You can send your units through the fog of war deep into the unexplored area, a dangerous land but also very rewarding. In there new resources sources can be found, trapped allies and others. But even while exploring the player must keep an eye on the base having in mind the hordes never stop coming.
Some of the mandatory quests will require your units to enter the fog of war in order to perform the task requested in the unexplored area.

#### Resource gathering
Some buildings are specifically made for that purpose, taking into account the upgrade level of the building it will gather a certain amount of the specified resource in a defined time space. There are various resource collecting buildings for some of the available resource.
Natural source are another way to gather resources that can’t be obtained through an specific building, in order to achieve those the player will have to explore through the fog of war.

#### Buildings creation and upgrading
The creation or upgrading of a building will consume an specific amount of resources and time, so the constant income of resources is vital in order to keep making the base grow. Even the training and creation of new units will require resources and time. Those factors are a very important mechanic to consider, the timing when it comes to create and upgrade a building or train a unit is essential, doing it while the enemy is attacking could result in a catastrophic outcome.
Buildings and units will have an specific resource to be upgraded so your base keeps growing up in terms of defensive and attacking stats, as the basic stats of buildings and units will improve at the same time the they get upgraded.

#### Unit Management
All units can move, individually or in group if you right click the terrain. Also, all units, except for recollectors, can attack if you right click an enemy unit, targeting an enemy forces the selected allies to walk towards him ignoring any other interference. Also, all the units that can attack will be able to set in defensive/guard mode. The unit will stop in the preselected zone by the player and if any enemy gets inside their attack range, that unit will start attacking until it dies or the enemy does so. Finally, the last action that only collectors can do is to repair. If your base and your buildings are damaged but not destroyed, all of them will recover their HP automatically with an amount of time. Otherwise and only if the building is destroyed, collectors will be able to repair it.

### Map
#### Exploration (Fog of War)
One of the game pillars is the exploration, not only focusing on fortifying your base also finding out other features and benefits that you can obtain of the surroundings.
Although it won’t be easy, around the base, covering the unexplored area, there will be a fog keeping the player off seeing further from a small radius, so the enemies will come from any possible angle without any previous warning.

Exploration will be a fundamental feature, the core of most of the quests will require to explore and even fight outside the base, we will include an increase of 
the difficulty as you advance stages.

The real difficulty here relies in the game forcing the player to keep an eye on the base every time he goes to explore since the hordes will keep coming no matter how far the player went to explore.

#### Enemy Position (Incursions)
Enemies won’t have an specific spawn location so the hordes will be less predictable. 

There will be other enemies spread around the map on the exploration zone, creating a deeper and more challenging quest and exploration system.}

### Rewards
- **2 kill quest**:
- **1 gather quest**:
- **1 delivery quests**: 
