# 🔌 TCP Chat Server

A multithreaded **TCP server and client** written in **C** for Linux. The server accepts multiple simultaneous client connections, handling each in a dedicated thread. Clients can exchange messages with the server interactively, with the server echoing messages back or responding to specific commands.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [How It Works](#how-it-works)
  - [Server](#server)
  - [Client](#client)
- [Prerequisites](#prerequisites)
- [Building](#building)
- [Running](#running)
  - [Start the Server](#start-the-server)
  - [Start a Client](#start-a-client)
  - [Testing Multiple Clients](#testing-multiple-clients)
- [Protocol](#protocol)
- [Troubleshooting](#troubleshooting)
- [License](#license)

---

## Overview

This project demonstrates low-level **POSIX socket programming** in C. The server listens on port `8080`, spawning a new thread per accepted connection so multiple clients can communicate concurrently without blocking each other. The client reads user input from stdin and exchanges messages with the server in a simple request-response loop.

---

## Features

- 🧵 **Multithreaded server** — each client connection runs in its own POSIX thread (`pthread`)
- 💬 **Interactive client** — reads from stdin, sends to server, prints the response
- 🔁 **Echo behaviour** — server echoes any message back to the sender
- 🤝 **Custom protocol** — server replies `"world"` when client sends `"hello"`
- 🚫 **Empty message filtering** — client silently ignores empty input without sending
- 🛑 **Graceful shutdown** — server catches `SIGINT` (`Ctrl+C`) and closes the socket cleanly
- 🔌 **Disconnect command** — client exits cleanly on `disconnect`
- ♻️ **`SO_REUSEADDR`** — server socket can be restarted immediately without waiting for OS timeout

---

## Project Structure

```
tcp-chat-server/
├── server/
│   └── server.c        # Multithreaded TCP server
├── client/
│   └── client.c        # Interactive TCP client
├── CMakeLists.txt       # CMake build configuration
├── .gitignore
└── README.md
```

---

## How It Works

### Server

```
startup
   │
   ├── socket()        — create TCP socket
   ├── setsockopt()    — SO_REUSEADDR to allow fast restart
   ├── bind()          — attach to port 8080
   ├── listen()        — mark socket as passive
   │
   └── loop: accept()
               │
               └── pthread_create() ──► handle_client()
                                            │
                                            ├── recv() message
                                            ├── if "hello" → send "world"
                                            ├── else       → echo back
                                            └── loop until client disconnects
```

A `SIGINT` handler closes the server socket, causing `accept()` to unblock and the main loop to exit cleanly.

### Client

```
startup
   │
   ├── socket()        — create TCP socket
   ├── connect()       — connect to server on port 8080
   │
   └── loop:
         ├── fgets()           — read line from stdin
         ├── skip if empty
         ├── if "disconnect"   → close socket, exit
         ├── send()            — send message to server
         └── recv() + print    — display server response
```

---

## Prerequisites

- Linux, macOS, or WSL (Windows Subsystem for Linux)
- GCC or Clang
- CMake ≥ 3.10
- POSIX threads (`pthread`) — available by default on Linux/macOS

---

## Building

```bash
git clone https://github.com/saidhann/TCP-echo.git
cd TCP-echo

mkdir build && cd build
cmake ..
make
```

This produces two executables in `build/`:
- `server`
- `client`

---

## Running

### Start the Server

```bash
./build/server
```

The server starts listening on `0.0.0.0:8080`. To stop it, press `Ctrl+C`.

### Start a Client

Open a separate terminal:

```bash
./build/client
```

Type any message and press Enter to send it. The server's response will be printed immediately. Type `disconnect` to exit.

```
> hello
world
> how are you
how are you
> disconnect
```

### Testing Multiple Clients

Open as many terminals as you like and run `./build/client` in each. The server handles all of them concurrently — messages from one client do not block or affect others.

```
Terminal 1          Terminal 2          Terminal 3
─────────────       ─────────────       ─────────────
> hello             > test              > foo
world               test                foo
```

---

## Protocol

The server implements a minimal text-based protocol over raw TCP:

| Client sends | Server responds |
|---|---|
| `hello` | `world` |
| anything else | (echoes the message back) |
| *(empty string)* | *(ignored, not sent)* |
| `disconnect` | *(client closes connection)* |

Messages are newline-terminated UTF-8 strings. There is no framing header — each `send()`/`recv()` call carries a single message.

---

## Troubleshooting

**`Address already in use` on startup**

Another process is already bound to port 8080. Find and stop it:

```bash
# Find the process using port 8080
sudo ss -tulpn | grep 8080

# Or with lsof
sudo lsof -i :8080

# Kill it by PID
kill -9 <PID>
```

Alternatively, change the port number in both `server.c` and `client.c`.

**Client connects but gets no response**

Make sure the server is running before starting the client. Check that both were built from the same source and are targeting the same port.

**Permission denied on bind**

Ports below 1024 are privileged on Linux. Port 8080 should not require elevated permissions — if you changed the port to something below 1024, either revert it or run with `sudo`.

---

## License

This project does not currently specify a license. Please contact the author before using or distributing this work.

---

> Built with ❤️ using C, POSIX sockets, and pthreads.
