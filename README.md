# fyzupix_koth

King of the Hill server-side mod for DayZ (adapted from MerkZone KOTH by Touchofdeath). Classes renamed to the `fyzupix_` namespace and the runtime config lives under the PackFazupix profile directory.

## Runtime config location

First boot creates:

```
$profile:/PackFazupix/KOTH/KOTHConfig.json
```

i.e. `<your -profiles= folder>/PackFazupix/KOTH/KOTHConfig.json`. Edit that JSON and restart the server; it is not hot-reloaded.

## Loot tiers

Rewards are picked with a weighted roll over `LootTiers[]`, then a random eligible item of that tier. Each item declares which `Tier` it belongs to.

```
"LootTiers": [
    { "Tier": 1, "Weight": 60, "Name": "Trash" },
    { "Tier": 2, "Weight": 25, "Name": "Common" },
    { "Tier": 3, "Weight": 10, "Name": "Rare" },
    { "Tier": 4, "Weight":  5, "Name": "Legendary" }
]
```

- `Weight` is **relative** (not a percentage). Ratios are what matters: `60/25/10/5` behaves the same as `6/2.5/1/0.5`.
- Tiers with no eligible items (i.e. every item in that tier failed its `Spawn_Chance` roll, or that tier has no items at all) are skipped and their weight drops out of the total for that slot.
- Setting a tier's `Weight` to `0` disables it.
- Leaving `LootTiers` empty falls back to the old flat-random pool (pure `Spawn_Chance`-based selection).

Each `Loot` entry now has a `Tier` field:

```
{ "ClassName": "SVD", "Spawn_Chance": 100, "Max_Spawnable": 2, "Quantity": -1, "Tier": 4, ... }
```

If `Tier` is omitted or `0`, the item is treated as tier `1`.

Per-item `Spawn_Chance` still runs **before** the tier roll, so you can both gate an item ("only 20% of the time this item is even in the pool") and control how often its tier is picked.

Logging (`Logging_Enabled: true` in settings) prints which tier was rolled for each reward slot:

```
[PackFazupix KOTH] Tier roll picked 'Legendary' (T4).
```

## Notifications

Notifications now fire on both paths when `Notification_Enabled` is true:

- `ExpansionNotification(...)` if compiled with `EXPANSIONMOD`.
- Vanilla `NotificationSystem.SendNotificationToPlayerExtended(...)` to every alive player — the **Notifications** Steam Workshop mod by DaemonForge (id `2353998362`) and the CommunityFramework both override this path, so players running those mods get styled toasts.

## Packaging

The tree is the PBO source. The prefix file is `$PBOPREFIX$` = `fyzupix_koth`.

Any DayZ-mod PBO packer works. Example with [armake2](https://github.com/KoffeinFlummi/armake2):

```
armake2 pack . fyzupix_koth.pbo
armake2 keygen creitinlapada
armake2 sign --signature creitinlapada creitinlapada.biprivatekey fyzupix_koth.pbo
```

The repo in `tacatapix/MOD` has a `tools/pack_pbo.py` that also works and does **not** require DayZ Tools — just:

```
python3 pack_pbo.py . fyzupix_koth.pbo
```

## Classes

- `fyzupix_KOTHflag` — the capture flag (uses the vanilla territory flag model).
- `fyzupix_KOTHchest_Base` / `_SupplyBox1..3` — the reward containers.

## Credit

Original script: **MerkZone KOTH by Touchofdeath**. This fork renames identifiers to the `fyzupix_` namespace, re-points the runtime config under `$profile:/PackFazupix/KOTH/`, adds configurable loot tiers, and routes notifications through both Expansion and the vanilla NotificationSystem (Notifications mod-compatible).
