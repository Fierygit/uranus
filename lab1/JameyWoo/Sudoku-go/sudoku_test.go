package Sudoku_go

import (
	"fmt"
	"io/ioutil"
	"strings"
	"testing"
	"time"
)

// 测试这个从博客中下载来的代码性能如何
func TestFastFromBlog(t *testing.T) {
	var sudoArr [9][9]int

	contentByte, err := ioutil.ReadFile("test1000")
	if err != nil {
		panic(err)
	}
	content := string(contentByte)
	lines := strings.Split(content, "\n")

	// 从这里开始计时
	start := time.Now()

	for _, line := range lines {
		//fmt.Println(line)
		for index, ch := range line {
			//fmt.Println(ch)
			sudoArr[index / 9][index % 9] = int(ch - 48)
		}
		data := New(sudoArr)
		data.Calc()
		//fmt.Printf("id %d 完成，猜测了%d次\n", id, data.guess_times)
		//for _, item := range data.sudokuList {
		//	fmt.Println(item)
		//}
	}

	end := time.Now()
	fmt.Println("use time:", end.Sub(start).Seconds(), "seconds")
}

