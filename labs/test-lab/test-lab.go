package main

import (
	"fmt"
	"os"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Error: You did not provide any arguments")
		return
	}
	name := ""

	for i := 1; i < len(os.Args); i++ {
		name += os.Args[i]
		if i+1 < len((os.Args)) {
			name += " "
		}
	}

	fmt.Printf("Hello %v, Welcome to the jungle\n", name)
}
