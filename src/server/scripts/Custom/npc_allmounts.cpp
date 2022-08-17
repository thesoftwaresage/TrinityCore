/*

# NPC - All Mounts #

_This module was created for [StygianCore](https://rebrand.ly/stygiancoreproject). A World of Warcraft 3.3.5a Solo/LAN repack by StygianTheBest | [GitHub](https://rebrand.ly/stygiangithub) | [Website](https://rebrand.ly/stygianthebest))_

### Description ###
------------------------------------------------------------------------------------------------------------------
- Adds an NPC that will teach all available mounts to the player

### To-Do ###
------------------------------------------------------------------------------------------------------------------
- Remove all mounts not compatible with 3.3.5a

### Data ###
------------------------------------------------------------------------------------------------------------------
- Type: NPC
- Script: All_Mounts_NPC
- Config: Yes
- SQL: Yes
    - NPC ID: 601014


### Version ###
------------------------------------------------------------------------------------------------------------------
- v2019.04.15 - Ported to AzerothCore by gtao725 (https://github.com/gtao725/)
- v2019.02.14 - Update AI, Fix Bengal Tiger, Remove Invalid Mounts
- v2018.12.15 - Updated for StygianCore
- v2017.09.03 - Added Bengal Tiger + Tiger Riding
- v2017.08.01 - Release


### CREDITS
------------------------------------------------------------------------------------------------------------------
![Styx](https://stygianthebest.github.io/assets/img/avatar/avatar-128.jpg "Styx")
![StygianCore](https://stygianthebest.github.io/assets/img/projects/stygiancore/StygianCore.png "StygianCore")

##### This module was created for [StygianCore](https://rebrand.ly/stygiancoreproject). A World of Warcraft 3.3.5a Solo/LAN repack by StygianTheBest | [GitHub](https://rebrand.ly/stygiangithub) | [Website](https://rebrand.ly/stygianthebest))

#### Additional Credits

- [Blizzard Entertainment](http://blizzard.com)
- [TrinityCore](https://github.com/TrinityCore/TrinityCore/blob/3.3.5/THANKS)
- [SunwellCore](http://www.azerothcore.org/pages/sunwell.pl/)
- [AzerothCore](https://github.com/AzerothCore/azerothcore-wotlk/graphs/contributors)
- [OregonCore](https://wiki.oregon-core.net/)
- [Wowhead.com](http://wowhead.com)
- [OwnedCore](http://ownedcore.com/)
- [ModCraft.io](http://modcraft.io/)
- [MMO Society](https://www.mmo-society.com/)
- [AoWoW](https://wotlk.evowow.com/)
- [More credits are cited in the sources](https://github.com/StygianTheBest)

### LICENSE
------------------------------------------------------------------------------------------------------------------
This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).

*/

#include "Config.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"

bool AllMountsAnnounceModule;
bool AllMountsTeachBengalTiger;
bool AllMountsEnableAI;

class AllMountsConfig : public WorldScript
{
public:
    AllMountsConfig() : WorldScript("AllMountsConfig_conf") { }

    void LoadFromConfigs(bool reload)
    {
        if (!reload) {
            AllMountsAnnounceModule = sConfigMgr->GetBoolDefault("AllMountsNPC.Announce", true);
            AllMountsTeachBengalTiger = sConfigMgr->GetBoolDefault("AllMountsNPC.TeachBengalTiger", false);
            AllMountsEnableAI = sConfigMgr->GetBoolDefault("AllMountsNPC.EnableAI", true);
        }
    }
};

class AllMountsAnnounce : public PlayerScript
{

public:

    AllMountsAnnounce() : PlayerScript("AllMountsAnnounce") {}

    void OnLogin(Player* player, bool loginFirst)
    {
        // Announce Module
        if (AllMountsAnnounceModule)
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00AllMountsNPC |rmodule.");
        }
    }
};

class All_Mounts_NPC : public CreatureScript
{

public:

    All_Mounts_NPC() : CreatureScript("All_Mounts_NPC") {}

    // Passive Emotes
    struct NPC_PassiveAI : public ScriptedAI
    {
        NPC_PassiveAI(Creature * creature) : ScriptedAI(creature) { }

        bool OnGossipHello(Player* player) override
        {

            std::ostringstream messageKnown;
            messageKnown << "I have already taught you all there is to know " << player->GetName() << ".";

            player->PlayerTalkClass->ClearMenus();

            // If they already know Furor's Fabled Steed, we've probably taught them everything before.
            if (player->HasSpell(24576))
            {
                AddGossipItemFor(player, GossipOptionIcon::None, messageKnown.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            }
            else
            {
                AddGossipItemFor(player, GossipOptionIcon::Trainer, "Teach me to ride.. EVERYTHING!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                AddGossipItemFor(player, GossipOptionIcon::None, "Maybe Next Time", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            }
            
            //SendGossipMenuFor(player, 601014, creature->GetGUID());

            player->TalkedToCreature(me->GetEntry(), me->GetGUID());
            SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            uint32 const uiAction = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            ClearGossipMenuFor(player);

            switch (uiAction)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:

                    // Teaches Bengal Tiger and Tiger Riding
                    // Disabled if using the BengalTiger NPC which teaches Tiger Riding
                    // on character creation. The tiger is then obtained in the cave.
                    if (AllMountsTeachBengalTiger)
                    {
                        player->LearnSpell(828, true);		// TigerRiding
                        player->LearnSpell(10790, true);		// ReinsoftheBengalTiger
                    }

                    // Add Items
                    player->AddItem(20221, 1);  // Add Furor's Fabled Steed

                    // Learn Mounts
                    player->LearnSpell(48778, true);	// AcherusDeathcharger
                    player->LearnSpell(60025, true);	// AlbinoDrake
                    player->LearnSpell(43688, true);	// AmaniWarBear
                    player->LearnSpell(16056, true);	// AncientFrostsaber
                    player->LearnSpell(16081, true);	// ArcticWolf
                    player->LearnSpell(66906, true);	// ArgentCharger
                    player->LearnSpell(63844, true);	// ArgentHippogryph
                    player->LearnSpell(66907, true);	// ArgentWarhorse
                    player->LearnSpell(67466, true);	// ArgentWarhorse
                    player->LearnSpell(61230, true);	// ArmoredBlueWindRider
                    player->LearnSpell(60114, true);	// ArmoredBrownBear
                    player->LearnSpell(60116, true);	// ArmoredBrownBear
                    player->LearnSpell(61229, true);	// ArmoredSnowyGryphon
                    player->LearnSpell(40192, true);	// AshesofAl'ar
                    player->LearnSpell(59567, true);	// AzureDrake
                    player->LearnSpell(41514, true);	// AzureNetherwingDrake
                    player->LearnSpell(51412, true);	// BigBattleBear
                    player->LearnSpell(58983, true);	// BigBlizzardBear
                    player->LearnSpell(71342, true);	// BigLoveRocket
                    player->LearnSpell(22719, true);	// BlackBattlestrider
                    player->LearnSpell(59650, true);	// BlackDrake
                    player->LearnSpell(35022, true);	// BlackHawkstrider
                    player->LearnSpell(16055, true);	// BlackNightsaber
                    player->LearnSpell(59976, true);	// BlackProto-Drake
                    player->LearnSpell(25863, true);	// BlackQirajiBattleTank
                    player->LearnSpell(26655, true);	// BlackQirajiBattleTank
                    player->LearnSpell(26656, true);	// BlackQirajiBattleTank
                    player->LearnSpell(17461, true);	// BlackRam
                    player->LearnSpell(64977, true);	// BlackSkeletalHorse
                    player->LearnSpell(470, true);	// BlackStallion
                    player->LearnSpell(60118, true);	// BlackWarBear
                    player->LearnSpell(60119, true);	// BlackWarBear
                    player->LearnSpell(48027, true);	// BlackWarElekk
                    player->LearnSpell(22718, true);	// BlackWarKodo
                    player->LearnSpell(59785, true);	// BlackWarMammoth
                    player->LearnSpell(59788, true);	// BlackWarMammoth
                    player->LearnSpell(22720, true);	// BlackWarRam
                    player->LearnSpell(22721, true);	// BlackWarRaptor
                    player->LearnSpell(22717, true);	// BlackWarSteed
                    player->LearnSpell(22723, true);	// BlackWarTiger
                    player->LearnSpell(22724, true);	// BlackWarWolf
                    player->LearnSpell(64658, true);	// BlackWolf
                    player->LearnSpell(74856, true);	// BlazingHippogryph
                    player->LearnSpell(72808, true);	// BloodbathedFrostbroodVanquisher
                    player->LearnSpell(61996, true);	// BlueDragonhawk
                    player->LearnSpell(59568, true);	// BlueDrake
                    player->LearnSpell(35020, true);	// BlueHawkstrider
                    player->LearnSpell(10969, true);	// BlueMechanostrider
                    player->LearnSpell(59996, true);	// BlueProto-Drake
                    player->LearnSpell(25953, true);	// BlueQirajiBattleTank
                    player->LearnSpell(39803, true);	// BlueRidingNetherRay
                    player->LearnSpell(17463, true);	// BlueSkeletalHorse
                    player->LearnSpell(64656, true);	// BlueSkeletalWarhorse
                    player->LearnSpell(32244, true);	// BlueWindRider
                    player->LearnSpell(50869, true);	// BrewfestKodo
                    player->LearnSpell(43899, true);	// BrewfestRam
                    player->LearnSpell(59569, true);	// BronzeDrake
                    player->LearnSpell(34406, true);	// BrownElekk
                    player->LearnSpell(458, true);	// BrownHorse
                    player->LearnSpell(18990, true);	// BrownKodo
                    player->LearnSpell(6899, true);	// BrownRam
                    player->LearnSpell(17464, true);	// BrownSkeletalHorse
                    player->LearnSpell(6654, true);	// BrownWolf
                    player->LearnSpell(58615, true);	// BrutalNetherDrake
                    player->LearnSpell(75614, true);	// CelestialSteed
                    player->LearnSpell(43927, true);	// CenarionWarHippogryph
                    player->LearnSpell(6648, true);	// ChestnutMare
                    player->LearnSpell(24576, true);	// Chromatic Drake
                    player->LearnSpell(41515, true);	// CobaltNetherwingDrake
                    player->LearnSpell(39315, true);	// CobaltRidingTalbuk
                    player->LearnSpell(34896, true);	// CobaltWarTalbuk
                    player->LearnSpell(73313, true);	// CrimsonDeathcharger
                    player->LearnSpell(68188, true);	// Crusader'sBlackWarhorse
                    player->LearnSpell(68187, true);	// Crusader'sWhiteWarhorse
                    player->LearnSpell(39316, true);	// DarkRidingTalbuk
                    player->LearnSpell(63635, true);	// DarkspearRaptor
                    player->LearnSpell(34790, true);	// DarkWarTalbuk
                    player->LearnSpell(63637, true);	// DarnassianNightsaber
                    player->LearnSpell(64927, true);	// DeadlyGladiator'sFrostWyrm
                    player->LearnSpell(6653, true);	// DireWolf
                    player->LearnSpell(32239, true);	// EbonGryphon
                    player->LearnSpell(63639, true);	// ExodarElekk
                    player->LearnSpell(36702, true);	// FieryWarhorse
                    player->LearnSpell(61451, true);	// FlyingCarpet
                    player->LearnSpell(44153, true);	// FlyingMachine
                    player->LearnSpell(63643, true);	// ForsakenWarhorse
                    player->LearnSpell(17460, true);	// FrostRam
                    player->LearnSpell(23509, true);	// FrostwolfHowler
                    player->LearnSpell(75596, true);	// FrostyFlyingCarpet
                    player->LearnSpell(65439, true);	// FuriousGladiator'sFrostWyrm
                    player->LearnSpell(63638, true);	// GnomereganMechanostrider
                    player->LearnSpell(32235, true);	// GoldenGryphon
                    player->LearnSpell(61465, true);	// GrandBlackWarMammoth
                    player->LearnSpell(61467, true);	// GrandBlackWarMammoth
                    player->LearnSpell(61469, true);	// GrandIceMammoth
                    player->LearnSpell(61470, true);	// GrandIceMammoth
                    player->LearnSpell(35710, true);	// GrayElekk
                    player->LearnSpell(18989, true);	// GrayKodo
                    player->LearnSpell(6777, true);	// GrayRam
                    player->LearnSpell(35713, true);	// GreatBlueElekk
                    player->LearnSpell(49379, true);	// GreatBrewfestKodo
                    player->LearnSpell(23249, true);	// GreatBrownKodo
                    player->LearnSpell(65641, true);	// GreatGoldenKodo
                    player->LearnSpell(23248, true);	// GreatGrayKodo
                    player->LearnSpell(35712, true);	// GreatGreenElekk
                    player->LearnSpell(35714, true);	// GreatPurpleElekk
                    player->LearnSpell(65637, true);	// GreatRedElekk
                    player->LearnSpell(23247, true);	// GreatWhiteKodo
                    player->LearnSpell(18991, true);	// GreenKodo
                    player->LearnSpell(17453, true);	// GreenMechanostrider
                    player->LearnSpell(61294, true);	// GreenProto-Drake
                    player->LearnSpell(26056, true);	// GreenQirajiBattleTank
                    player->LearnSpell(39798, true);	// GreenRidingNetherRay
                    player->LearnSpell(17465, true);	// GreenSkeletalWarhorse
                    player->LearnSpell(32245, true);	// GreenWindRider
                    player->LearnSpell(48025, true);	// HeadlessHorseman'sMount
                    player->LearnSpell(72807, true);	// IceboundFrostbroodVanquisher
                    player->LearnSpell(59797, true);	// IceMammoth
                    player->LearnSpell(59799, true);	// IceMammoth
                    player->LearnSpell(17459, true);	// IcyBlueMechanostriderModA
                    player->LearnSpell(72286, true);	// Invincible'sReins
                    player->LearnSpell(63956, true);	// IronboundProto-Drake
                    player->LearnSpell(63636, true);	// IronforgeRam
                    player->LearnSpell(17450, true);	// IvoryRaptor
                    player->LearnSpell(65917, true);	// MagicRooster
                    player->LearnSpell(61309, true);	// MagnificentFlyingCarpet
                    player->LearnSpell(55531, true);	// Mechano-Hog
                    player->LearnSpell(60424, true);	// Mekgineer'sChopper
                    player->LearnSpell(44744, true);	// MercilessNetherDrake
                    player->LearnSpell(63796, true);	// Mimiron'sHead
                    player->LearnSpell(16084, true);	// MottledRedRaptor
                    player->LearnSpell(66846, true);	// OchreSkeletalWarhorse
                    player->LearnSpell(69395, true);	// OnyxianDrake
                    player->LearnSpell(41513, true);	// OnyxNetherwingDrake
                    player->LearnSpell(63640, true);	// OrgrimmarWolf
                    player->LearnSpell(16082, true);	// Palomino
                    player->LearnSpell(32345, true);	// PeepthePhoenixMount
                    player->LearnSpell(472, true);	// Pinto
                    player->LearnSpell(60021, true);	// PlaguedProto-Drake
                    player->LearnSpell(35711, true);	// PurpleElekk
                    player->LearnSpell(35018, true);	// PurpleHawkstrider
                    player->LearnSpell(41516, true);	// PurpleNetherwingDrake
                    player->LearnSpell(39801, true);	// PurpleRidingNetherRay
                    player->LearnSpell(23246, true);	// PurpleSkeletalWarhorse
                    player->LearnSpell(66090, true);	// Quel'doreiSteed
                    player->LearnSpell(41252, true);	// RavenLord
                    player->LearnSpell(61997, true);	// RedDragonhawk
                    player->LearnSpell(59570, true);	// RedDrake
                    player->LearnSpell(34795, true);	// RedHawkstrider
                    player->LearnSpell(10873, true);	// RedMechanostrider
                    player->LearnSpell(59961, true);	// RedProto-Drake
                    player->LearnSpell(26054, true);	// RedQirajiBattleTank
                    player->LearnSpell(39800, true);	// RedRidingNetherRay
                    player->LearnSpell(17462, true);	// RedSkeletalHorse
                    player->LearnSpell(22722, true);	// RedSkeletalWarhorse
                    player->LearnSpell(16080, true);	// RedWolf
                    player->LearnSpell(67336, true);	// RelentlessGladiator'sFrostWyrm
                    player->LearnSpell(30174, true);	// RidingTurtle
                    player->LearnSpell(17481, true);	// Rivendare'sDeathcharger
                    player->LearnSpell(63963, true);	// RustedProto-Drake
                    player->LearnSpell(64731, true);	// SeaTurtle
                    player->LearnSpell(66087, true);	// SilverCovenantHippogryph
                    player->LearnSpell(63642, true);	// SilvermoonHawkstrider
                    player->LearnSpell(39802, true);	// SilverRidingNetherRay
                    player->LearnSpell(39317, true);	// SilverRidingTalbuk
                    player->LearnSpell(34898, true);	// SilverWarTalbuk
                    player->LearnSpell(32240, true);	// SnowyGryphon
                    player->LearnSpell(42776, true);	// SpectralTiger
                    player->LearnSpell(10789, true);	// SpottedFrostsaber
                    player->LearnSpell(23510, true);	// StormpikeBattleCharger
                    player->LearnSpell(63232, true);	// StormwindSteed
                    player->LearnSpell(66847, true);	// StripedDawnsaber
                    player->LearnSpell(10793, true);	// StripedNightsaber
                    player->LearnSpell(66088, true);	// SunreaverDragonhawk
                    player->LearnSpell(66091, true);	// SunreaverHawkstrider
                    player->LearnSpell(68057, true);	// SwiftAllianceSteed
                    player->LearnSpell(32242, true);	// SwiftBlueGryphon
                    player->LearnSpell(23241, true);	// SwiftBlueRaptor
                    player->LearnSpell(43900, true);	// SwiftBrewfestRam
                    player->LearnSpell(23238, true);	// SwiftBrownRam
                    player->LearnSpell(23229, true);	// SwiftBrownSteed
                    player->LearnSpell(23250, true);	// SwiftBrownWolf
                    player->LearnSpell(65646, true);	// SwiftBurgundyWolf
                    player->LearnSpell(23221, true);	// SwiftFrostsaber
                    player->LearnSpell(23239, true);	// SwiftGrayRam
                    player->LearnSpell(65640, true);	// SwiftGraySteed
                    player->LearnSpell(23252, true);	// SwiftGrayWolf
                    player->LearnSpell(32290, true);	// SwiftGreenGryphon
                    player->LearnSpell(35025, true);	// SwiftGreenHawkstrider
                    player->LearnSpell(23225, true);	// SwiftGreenMechanostrider
                    player->LearnSpell(32295, true);	// SwiftGreenWindRider
                    player->LearnSpell(68056, true);	// SwiftHordeWolf
                    player->LearnSpell(23219, true);	// SwiftMistsaber
                    player->LearnSpell(65638, true);	// SwiftMoonsaber
                    player->LearnSpell(37015, true);	// SwiftNetherDrake
                    player->LearnSpell(23242, true);	// SwiftOliveRaptor
                    player->LearnSpell(23243, true);	// SwiftOrangeRaptor
                    player->LearnSpell(23227, true);	// SwiftPalomino
                    player->LearnSpell(33660, true);	// SwiftPinkHawkstrider
                    player->LearnSpell(32292, true);	// SwiftPurpleGryphon
                    player->LearnSpell(35027, true);	// SwiftPurpleHawkstrider
                    player->LearnSpell(65644, true);	// SwiftPurpleRaptor
                    player->LearnSpell(32297, true);	// SwiftPurpleWindRider
                    player->LearnSpell(24242, true);	// SwiftRazzashiRaptor
                    player->LearnSpell(32289, true);	// SwiftRedGryphon
                    player->LearnSpell(65639, true);	// SwiftRedHawkstrider
                    player->LearnSpell(32246, true);	// SwiftRedWindRider
                    player->LearnSpell(55164, true);	// SwiftSpectralGryphon
                    player->LearnSpell(42777, true);	// SwiftSpectralTiger
                    player->LearnSpell(23338, true);	// SwiftStormsaber
                    player->LearnSpell(23251, true);	// SwiftTimberWolf
                    player->LearnSpell(65643, true);	// SwiftVioletRam
                    player->LearnSpell(35028, true);	// SwiftWarstrider
                    player->LearnSpell(46628, true);	// SwiftWhiteHawkstrider
                    player->LearnSpell(23223, true);	// SwiftWhiteMechanostrider
                    player->LearnSpell(23240, true);	// SwiftWhiteRam
                    player->LearnSpell(23228, true);	// SwiftWhiteSteed
                    player->LearnSpell(23222, true);	// SwiftYellowMechanostrider
                    player->LearnSpell(32296, true);	// SwiftYellowWindRider
                    player->LearnSpell(49322, true);	// SwiftZhevra
                    player->LearnSpell(24252, true);	// SwiftZulianTiger
                    player->LearnSpell(39318, true);	// TanRidingTalbuk
                    player->LearnSpell(34899, true);	// TanWarTalbuk
                    player->LearnSpell(32243, true);	// TawnyWindRider
                    player->LearnSpell(18992, true);	// TealKodo
                    player->LearnSpell(63641, true);	// ThunderBluffKodo
                    player->LearnSpell(580, true);	// TimberWolf
                    player->LearnSpell(60002, true);	// Time-LostProto-Drake
                    player->LearnSpell(61425, true);	// Traveler'sTundraMammoth
                    player->LearnSpell(61447, true);	// Traveler'sTundraMammoth
                    player->LearnSpell(44151, true);	// Turbo-ChargedFlyingMachine
                    player->LearnSpell(65642, true);	// Turbostrider
                    player->LearnSpell(10796, true);	// TurquoiseRaptor
                    player->LearnSpell(59571, true);	// TwilightDrake
                    player->LearnSpell(17454, true);	// UnpaintedMechanostrider
                    player->LearnSpell(49193, true);	// VengefulNetherDrake
                    player->LearnSpell(64659, true);	// VenomhideRavasaur
                    player->LearnSpell(41517, true);	// VeridianNetherwingDrake
                    player->LearnSpell(41518, true);	// VioletNetherwingDrake
                    player->LearnSpell(60024, true);	// VioletProto-Drake
                    player->LearnSpell(10799, true);	// VioletRaptor
                    player->LearnSpell(64657, true);	// WhiteKodo
                    player->LearnSpell(15779, true);	// WhiteMechanostriderModB
                    player->LearnSpell(54753, true);	// WhitePolarBear
                    player->LearnSpell(6898, true);	// WhiteRam
                    player->LearnSpell(39319, true);	// WhiteRidingTalbuk
                    player->LearnSpell(65645, true);	// WhiteSkeletalWarhorse
                    player->LearnSpell(16083, true);	// WhiteStallion
                    player->LearnSpell(34897, true);	// WhiteWarTalbuk
                    player->LearnSpell(54729, true);	// WingedSteedoftheEbonBlade
                    player->LearnSpell(17229, true);	// WinterspringFrostsaber
                    player->LearnSpell(59791, true);	// WoolyMammoth
                    player->LearnSpell(59793, true);	// WoolyMammoth
                    player->LearnSpell(74918, true);	// WoolyWhiteRhino
                    player->LearnSpell(71810, true);	// WrathfulGladiator'sFrostWyrm
                    player->LearnSpell(46197, true);	// X-51Nether-Rocket
                    player->LearnSpell(46199, true);	// X-51Nether-RocketX-TREME
                    player->LearnSpell(75973, true);	// X-53TouringRocket
                    player->LearnSpell(26055, true);	// YellowQirajiBattleTank

                    // Goodbye
                    CloseGossipMenuFor(player);
                    break;

                case GOSSIP_ACTION_INFO_DEF + 2:

                    // Goodbye
                    CloseGossipMenuFor(player);
                    break;
            }

            return true;
        }

        uint32 Choice;
        uint32 MessageTimer;

        // Called once when client is loaded
        void Reset()
        {
            MessageTimer = urand(60000, 180000); // 1-3 minutes
        }

        // Called at World update tick
        void UpdateAI(const uint32 diff)
        {
            if (AllMountsEnableAI)
            {
                if (MessageTimer <= diff)
                {
                    // Make a random message choice
                    Choice = urand(1, 3);

                    switch (Choice)
                    {
                    case 1:
                    {
                        me->Say("I can teach you to ride.. anything!", LANG_UNIVERSAL);
                        me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
                        MessageTimer = urand(60000, 180000);
                        break;
                    }
                    case 2:
                    {
                        me->Say("Have you ever wanted to mount a chicken?", LANG_UNIVERSAL);
                        me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
                        MessageTimer = urand(60000, 180000);
                        break;
                    }
                    case 3:
                    {
                        me->Say("The finest mounts in all of Azeroth are in my stables.", LANG_UNIVERSAL);
                        me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
                        MessageTimer = urand(60000, 180000);
                        break;
                    }
                    default:
                    {
                        me->Say("The finest mounts in all of Azeroth are in my stables.", LANG_UNIVERSAL);
                        me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
                        MessageTimer = urand(60000, 180000);
                        break;
                    }
                    }
                }
                else { MessageTimer -= diff; }
            }
        };
    };

    // CREATURE AI
    CreatureAI * GetAI(Creature * creature) const override
    {
        return new NPC_PassiveAI(creature);
    }
};

void AddAllMountsNPCScripts()
{
    new AllMountsConfig();
    new AllMountsAnnounce();
    new All_Mounts_NPC();
}
