// Clock Server is a concurrent TCP server that periodically writes the time.
package main

import (
	"flag"
	"io"
	"log"
	"net"
	"os"
	"strconv"
	"time"
)

func handleConn(c net.Conn, tz string) {
	defer c.Close()

	loc, err := time.LoadLocation(tz)
	if err != nil {
		println(tz, "ERROR: Time zone unknown")
		c.Close()
		return
	}

	for {

		if _, err := io.WriteString(c, time.Now().In(loc).Format("15:04:05\n")); err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	var port = flag.Int("port", 9090, "Port to run the server on")
	flag.Parse()
	var TZ = os.Getenv("TZ")
	if TZ == "" {
		println("ERROR: No env variable TZ provided")
		return
	}
	listener, err := net.Listen("tcp", "localhost:"+strconv.Itoa(*port))
	println("Server up in", "localhost:", *port, "With time zone:", TZ)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn, TZ) // handle connections concurrently
	}
}
