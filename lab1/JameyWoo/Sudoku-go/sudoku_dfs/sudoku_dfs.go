package main

import (
	"fmt"
	"io/ioutil"
	"strings"
	"sync"
	"time"
)

var sign = false

var Sudokus [][9][9]int
var Sudoku [9][9]int

func getSudoku( ) [][9][9]int {
	var sudo [][9][9]int
	contentByte, err := ioutil.ReadFile("../test1000")
	if err != nil {
		panic(err)
	}
	content := string(contentByte)
	lines := strings.Split(content, "\n")
	var sudoArr [9][9]int

	for _, line := range lines {
		//fmt.Println(line)
		for index, ch := range line {
			//fmt.Println(ch)
			sudoArr[index / 9][index % 9] = int(ch - 48)
		}
		sudo = append(sudo, sudoArr)
	}
	return sudo
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

func main() {
	var wg sync.WaitGroup
	Sudokus = getSudoku()
	cnt := 10
	//chans := make([]chan bool, cnt)
	for i := 0; i < cnt; i++ {
		wg.Add(1)
		sign = false
		go func(ii int, Sudokus [][9][9]int) {
			sudoku := Sudokus[ii]
			start := time.Now()
			DFS(0)
			end := time.Now()
			//chans[ii] <- true
			fmt.Println(end.Sub(start))
			Output(sudoku)
			//defer wg.Done()
		}(i, Sudokus)
	}

	//wg.Wait()
	time.Sleep(time.Minute)
	//for i := 0; i < cnt; i++ {
	//	<- chans[i]
	//}
}