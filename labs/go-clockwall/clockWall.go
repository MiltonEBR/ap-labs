package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"sort"
	"strings"
)

type Clock struct {
	Zone string
	Time string
}

func main() {

	args := os.Args[1:]
	activeClocks := make(chan Clock)
	clockClosed := make(chan bool, 1)

	clocks := len(args)

	m := make(map[string]string)
	for _, arg := range args {
		parsed := strings.Split(arg, "=")
		if len(parsed) != 2 {
			log.Fatal("Invalid argument: " + arg + " please use <Time Zone>:<Address>")
		}
		//m[parsed[0]] = "No hour received"
		conn, err := net.Dial("tcp", parsed[1])
		if err != nil {
			log.Fatal("Could not connect to " + parsed[1])
		}
		defer conn.Close()

		go mustCopy(conn, activeClocks, clockClosed)
	}

	closed := 0

	go displayTimes(activeClocks, m)

	for {
		x := <-clockClosed
		if x {
			closed++
			if closed >= clocks {
				close(activeClocks)
				close(clockClosed)
				fmt.Println("All clocks have disconnected, closing program...")
				return
			}
		}
	}

}

func mustCopy(src io.Reader, activeClocks chan Clock, clockClosed chan bool) {

	buf := make([]byte, 100)
	zone := ""
	for {
		n, err := src.Read(buf)

		if err != nil || n <= 0 {
			clockClosed <- true
			if zone != "" {
				clock := new(Clock)
				clock.Zone = zone
				clock.Time = "Clock disconnected"
				activeClocks <- *clock
			}
			return
		}
		parsedRes := strings.Split(string(buf[:n]), ",")
		clock := new(Clock)
		clock.Zone = parsedRes[1]
		zone = parsedRes[1]
		clock.Time = parsedRes[0]
		activeClocks <- *clock
	}
}

func displayTimes(clocks chan Clock, m map[string]string) {

	for x := range clocks {
		m[x.Zone] = x.Time
		fmt.Print("\033[H\033[2J")
		keys := make([]string, 0, len(m))
		for k := range m {
			keys = append(keys, k)
		}
		sort.Strings(keys)

		for _, k := range keys {
			fmt.Printf("%s : %s\n", k, m[k])
		}
	}
}
