package main

import (
	"fmt"
)

var sign = false

var Sudoku = [9][9]int{
	{0, 0, 0, 0, 0, 2, 0, 5, 0},
	{0, 7, 8, 0, 0, 0, 3, 0, 0},
	{0, 0, 0, 0, 0, 4, 0, 0, 0},
	{5, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 3, 0, 7, 0, 8},
	{2, 0, 0, 0, 0, 0, 0, 4, 0},
	{0, 0, 0, 0, 0, 5, 0, 9, 0},
	{0, 1, 0, 0, 7, 0, 0, 0, 0},
}

func main() {
	DFS(0)
	Output(Sudoku)
}

func Output(sudoku [9][9]int) {
	for i := 0; i < 81; i++ {
		fmt.Printf("%2d ", sudoku[i/9][i%9])
		if i != 0 && (i+1)%9 == 0 {
			fmt.Println("")
		}
	}
}

func Check(n int, key int) bool {
	for i := 0; i < 9; i++ {
		r := n / 9
		if Sudoku[r][i] == key {
			return false
		}
	}

	for i := 0; i < 9; i++ {
		c := n % 9
		if Sudoku[i][c] == key {
			return false
		}
	}

	x := n / 9 / 3 * 3
	y := n % 9 / 3 * 3

	for i := x; i < x+3; i++ {
		for j := y; j < y+3; j++ {
			if Sudoku[i][j] == key {
				return false
			}
		}
	}

	return true
}

func DFS(n int) {
	if n > 80 {
		sign = true
		return
	}

	if Sudoku[n/9][n%9] != 0 {
		DFS(n + 1)
	} else {
		for v := 1; v <= 9; v++ {
			if Check(n, v) {
				Sudoku[n/9][n%9] = v
				DFS(n + 1)
				if sign {
					return
				}
				Sudoku[n/9][n%9] = 0
			}
		}
	}
}
