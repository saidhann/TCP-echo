#!/bin/bash

SERVER_ADDRESS="127.0.0.1"
SERVER_PORT="8080"

# Function to send messages
send_message() {
    local message="$1"
    echo -e "$message\n" | nc $SERVER_ADDRESS $SERVER_PORT
}

# Send messages
send_message "Client 1: Hello from client 1"
send_message "Client 2: Hello from client 2"
send_message "Client 3: Hello from client 3"