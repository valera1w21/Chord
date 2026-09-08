# Chord

Two voices, one line.

A minimal peer-to-peer voice chat for exactly two people. Written in C.
No browser engine, no accounts, no company in the middle.

**Status:** learning project, work in progress. Nothing works yet.

## Why

Talking to one person over the internet should be simple. It is not:

- **Discord** is blocked in some countries, requires an account, and spends
  hundreds of megabytes of RAM to show a window.
- **Mumble** has excellent voice quality and can be self-hosted, but it cannot
  share a screen.
- **Jitsi** can share a screen, but it is a full browser stack you have to host
  and maintain.

There is no small, native program that does voice *and* screen sharing for
exactly two people. So I am writing one.

## Scope

What Chord does:

- Call one person directly by address
- Two-way voice
- Mute
- Screen sharing, with optional system audio
- Hang up

What Chord will never do:

- Groups, channels, or servers to join
- Accounts, registration, or friend lists
- Text chat, history, bots, emoji

Two people. One line. Nothing else.

## Roadmap

- [ ] UDP transport with tagged packets
- [ ] Microphone capture and playback
- [ ] Voice over LAN
- [ ] Opus encoding
- [ ] Jitter buffer
- [ ] Encryption
- [ ] NAT traversal and rendezvous server
- [ ] Call control: ring, answer, mute, hang up
- [ ] Screen sharing
- [ ] Graphical interface

## Build

Windows, MSVC (Visual Studio Build Tools).
Open "x64 Native Tools Command Prompt" and run:
`cl /nologo /W4 /Zi /utf-8 recv.c ws2_32.lib`

## Usage

Not usable yet.

## How it works

Chord is a *dyad*, not a server: a group of two that exists only while both
people are there. Voice and screen travel directly between the two machines.
A small rendezvous server is used only to introduce the two sides to each
other; it carries the conversation itself only as a fallback, when a direct
connection cannot be established.

The transport does not know what it carries. Every packet has a type tag and a
sequence number, so voice, screen, and control messages share one connection
and can be prioritised independently.

## License

To be decided.