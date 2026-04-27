# Chip-8 Emulator

Un émulateur **Chip-8** écrit en **C**.

Le Chip-8 est une machine virtuelle des années 70 utilisée pour faire tourner de petits jeux. Ce projet réimplémente son interpréteur : chargement d'une ROM, lecture des opcodes, et exécution des instructions.

## Fonctionnalités

- Initialisation de la console (mémoire, registres, pile, fontset)
- Cycle **fetch / decode / execute**
- Implémentation des opcodes principaux (dont `0xD` pour le rendu graphique)
- Chargement de ROMs `.ch8`

## Compilation

```bash
make
```

## Utilisation

```bash
./emulateur8 IBM_Logo.ch8
```

## Auteur

Yanis
