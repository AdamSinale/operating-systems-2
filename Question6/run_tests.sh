#!/bin/bash

# Create input file for the server
echo "This is a test input for server." > input_file.txt

# Run the TCP server in the background
./mynetcat -e "ttt 123456789" -b TCPS4050 &
sleep 3
# Run the TCP client in the foreground and send "4 5 0" as input
echo "3 3 4 9 8 0" | ./mynetcat -e "ttt 123456789" -o TCPClocalhost,4050 

sleep 2

# Run the TCP server in the background
./mynetcat -e "ttt 123456789" -b TCPS4050 &
sleep 3
# Run the TCP client in the foreground and send "4 5 0" as input
echo "3 3 6 9 0" | ./mynetcat -e "ttt 123456789" -o TCPClocalhost,4050 

sleep 2

# Run the TCP server in the background
./mynetcat -e "ttt 123456789" -b TCPS4050 &
sleep 3
# Run the TCP client in the foreground and send "4 5 9 8" as input
echo "4 4 5 0" | ./mynetcat -e "ttt 123456789" -o TCPClocalhost,4050 

sleep 2

# ----------------------------------------------------------------------

# Run the TCP server in the background
./mynetcat -e "ttt 123456789" -i TCPS4050 &
sleep 3
# Run the TCP client in the foreground and send "4 5 0" as input
echo "4 4 5 0" | ./mynetcat -e "ttt 123456789" -o TCPClocalhost,4050 

sleep 2

# Run the UDP server in the background
./mynetcat -e "ttt 123456789" -i UDPS4050 &
sleep 3
# Run the UDP client in the foreground and send "4 5 0" as input
echo "4 4 5 0" | ./mynetcat -e "ttt 123456789" -o UDPClocalhost,4050 

sleep 2

# Run the UDS server (datagram) in the background
./mynetcat -e "ttt 123456789" -i UDSSD"/tmp/socket1" input_file.txt &
sleep 3
# Run the UDS client (datagram) in the foreground and send "4 5 0" as input
echo "4 4 5 0" | ./mynetcat -e "ttt 123456789" -o UDSCD"/tmp/socket1" client_output.txt

sleep 2

# Run the UDS server (stream) in the background
./mynetcat -e "ttt 123456789" -i UDSSS"/tmp/socket2" input_file.txt &
sleep 3
# Run the UDS client (stream) in the foreground and send "4 5 0" as input
echo "4 4 5 0" | ./mynetcat -e "ttt 123456789" -o UDSCS"/tmp/socket2" client_output.txt

sleep 2

# --------------------------------------------------------------------------------------------

# Run the TCP server in the background
./mynetcat -e "ttt 123456789" -i TCPS4050 &
sleep 3
# Run the TCP client in the foreground and send "4 5 0" as input
echo "3 3 6 9 0" | ./mynetcat -e "ttt 123456789" -o TCPClocalhost,4050 

sleep 2

# Run the UDP server in the background
./mynetcat -e "ttt 123456789" -i UDPS4050 &
sleep 3
# Run the UDP client in the foreground and send "3 6 9 0" as input
echo "3 3 6 9 0" | ./mynetcat -e "ttt 123456789" -o UDPClocalhost,4050 

sleep 2

# Run the UDS server (datagram) in the background
./mynetcat -e "ttt 123456789" -i UDSSD"/tmp/socket1" input_file.txt &
sleep 3
# Run the UDS client (datagram) in the foreground and send "3 6 9 0" as input
echo "3 3 6 9 0" | ./mynetcat -e "ttt 123456789" -o UDSCD"/tmp/socket1" client_output.txt

sleep 2

# Run the UDS server (stream) in the background
./mynetcat -e "ttt 123456789" -i UDSSS"/tmp/socket2" input_file.txt &
sleep 3
# Run the UDS client (stream) in the foreground and send "3 6 9 0" as input
echo "3 3 6 9 0" | ./mynetcat -e "ttt 123456789" -o UDSCS"/tmp/socket2" client_output.txt
sleep 2

# --------------------------------------------------------------------------------------------

# Run the TCP server in the background
./mynetcat -e "ttt 123456789" -i TCPS4050 &
sleep 3
# Run the TCP client in the foreground and send "4 5 9 8" as input
echo "3 3 4 9 8 0" | ./mynetcat -e "ttt 123456789" -o TCPClocalhost,4050 

sleep 2

# Run the UDP server in the background
./mynetcat -e "ttt 123456789" -i UDPS4050 &
sleep 3
# Run the UDP client in the foreground and send "4 5 9 8" as input
echo "3 3 4 9 8 0" | ./mynetcat -e "ttt 123456789" -o UDPClocalhost,4050 

sleep 2

# Run the UDS server (datagram) in the background
./mynetcat -e "ttt 123456789" -i UDSSD"/tmp/socket1" input_file.txt &
sleep 3
# Run the UDS client (datagram) in the foreground and send "4 5 9 8" as input
echo "3 3 4 9 8 0" | ./mynetcat -e "ttt 123456789" -o UDSCD"/tmp/socket1" client_output.txt

sleep 2

# Run the UDS server (stream) in the background
./mynetcat -e "ttt 123456789" -i UDSSS"/tmp/socket2" input_file.txt &
sleep 4
# Run the UDS client (stream) in the foreground and send "4 5 9 8" as input
echo "3 3 4 9 8 0" | ./mynetcat -e "ttt 123456789" -o UDSCS"/tmp/socket2" client_output.txt

sleep 5
