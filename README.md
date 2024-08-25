# TCP Server and Client

This project contains a simple TCP server and client implementation in C. The server can handle multiple client connections concurrently using threads. The client communicates with the server, sending messages and receiving responses.

## Files

- **`server.c`**: TCP server code that handles multiple clients.
- **`client.c`**: TCP client code to interact with the server.

## Features

- The server responds with "world" when a client sends the message "hello".
- The server echoes any other message back to the client.
- Both server and client handle disconnections gracefully.
- The client ignores and does not send empty messages.

## Prerequisites

- Linux or macOS environment (or Windows Subsystem for Linux)
- GCC compiler
- CMake (for build automation)

## Compilation

To compile the server and client programs, use CMake. Ensure you have CMake installed.

1. **Create a build directory and compile:**

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

   This will produce the `server` and `client` executables in the `build` directory.

## Running the Server

1. **Start the server:**

    ```bash
    ./server
    ```

   The server will start listening on port 8080.

## Running the Client

1. **Start the client in a separate terminal:**

    ```bash
    ./client
    ```

2. **Enter messages to send to the server.**
    - Type your message and press Enter.
    - Type `disconnect` to close the connection and exit the client.

## Testing Multiple Clients

1. **Open multiple terminals.**
2. **In each terminal, run the client program:**

    ```bash
    ./client
    ```

3. **Enter different messages from each client and observe the server's responses.**

## Code Explanation

### Server Code (`server.c`)

- **Socket Creation:** Creates a TCP socket using `socket()`.
- **Socket Options:** Sets the `SO_REUSEADDR` option to allow reuse of the address.
- **Binding:** Binds the socket to port 8080.
- **Listening:** Listens for incoming connections.
- **Accepting Connections:** Accepts client connections and handles each in a new thread.
- **Handling Clients:** Receives messages from clients, responds with "world" for "hello", and echoes other messages.
- **Graceful Shutdown:** Handles SIGINT to close the server socket gracefully.

### Client Code (`client.c`)

- **Socket Creation:** Creates a TCP socket using `socket()`.
- **Connecting:** Connects to the server on port 8080.
- **Sending Messages:** Reads user input, sends messages to the server, and handles empty messages.
- **Receiving Responses:** Receives and displays responses from the server.
- **Disconnection:** Closes the connection and exits when "disconnect" is typed.

## Troubleshooting

- **Address Already in Use:** If you get an error about port 8080 being in use, make sure no other instance of the server is running. Use the commands provided in the troubleshooting section to find and terminate the process using port 8080.

- **Permission Issues:** You might need elevated permissions to bind to some ports. Try using a different port if you encounter permission errors.

## License

No license `:)`
