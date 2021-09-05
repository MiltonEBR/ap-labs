// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.

//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

func (p Point) X() float64 {
	return p.x
}
func (p Point) Y() float64 {
	return p.y
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

func GenRandomPoint(rang float64) Point {
	rand.Seed(time.Now().UnixNano())

	x, y := rand.Float64()*rang, rand.Float64()*rang
	if rand.Float32() > 0.5 {
		x = -x
	}
	if rand.Float32() > 0.5 {
		y = -y
	}

	return Point{x, y}

}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 { //Now returns perimeter
	sum := 0.0
	for i := range path {
		if i > 0 {
			dist := path[i-1].Distance(path[i])
			sum += dist
			fmt.Printf("%f + ", dist)

		}
	}

	//Last side
	dist := path[0].Distance(path[len(path)-1])
	sum += dist
	fmt.Printf("%f = %f\n", dist, sum)
	return sum
}

func CheckIntersect(p1, q1, p2, q2 Point) bool {
	onSegment := func(p, q, r Point) bool {
		if q.x <= math.Max(p.x, r.x) && q.x >= math.Min(p.x, r.x) && q.y <= math.Max(p.y, r.y) && q.y >= math.Min(p.y, r.y) {
			return true
		}
		return false
	}

	orientation := func(p, q, r Point) int {
		val := (q.y-p.y)*(r.x-q.x) - (q.x-p.x)*(r.y-q.y)

		if val == 0 {
			return 0
		}

		if val > 0 {
			return 1
		}
		return 2
	}

	o1 := orientation(p1, q1, p2)
	o2 := orientation(p1, q1, q2)
	o3 := orientation(p2, q2, p1)
	o4 := orientation(p2, q2, q1)

	if o1 != o2 && o3 != o4 {
		return true
	}

	if o1 == 0 && onSegment(p1, p2, q1) {
		return true
	}

	if o2 == 0 && onSegment(p1, q2, q1) {
		return true
	}

	if o3 == 0 && onSegment(p2, p1, q2) {
		return true
	}

	if o4 == 0 && onSegment(p2, q1, q2) {
		return true
	}

	return false
}

func createFigure(sides int) Path {

	var fig = make(Path, sides)

	fmt.Printf("Generating a [%d] sides figure\n", sides)
	fmt.Println("Figure's vertices")

	for i := 0; i < sides; i++ {

		fig[i] = GenRandomPoint(float64(100))

		if i > 3 {
			intersect := CheckIntersect(fig[i-3], fig[i-2], fig[i-1], fig[i])
			if i+1 <= sides {

				for intersect {
					fig[i] = GenRandomPoint(float64(100))
					intersect = CheckIntersect(fig[i-3], fig[i-2], fig[i-1], fig[i])
				}

			} else { //Check if last point also has no collision when joining first point

				for intersect {
					fig[i] = GenRandomPoint(float64(100))
					intersect = CheckIntersect(fig[i-3], fig[i-2], fig[i-1], fig[i]) && CheckIntersect(fig[i-2], fig[i-1], fig[i], fig[0])
				}

			}
		}
		fmt.Printf("(%f, %f)\n", fig[i].x, fig[i].y)
	}

	return fig
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Please provide a proper number of sides")
		return
	}

	sides, err := strconv.Atoi(os.Args[1])

	if err != nil {
		fmt.Println("Error with provided 'sides' argument")
		return
	}

	if sides < 3 {
		fmt.Println("Must provide at least 3 sides to make a figure")
		return
	}

	fig := createFigure(sides)
	fmt.Printf("Figure's Perimeter\n")
	fig.Distance()

}

//!-path
