package main

import (
	"io/ioutil"
	"os"
	"strings"
)

func main() {
	contentByte, err := ioutil.ReadFile("../test100")
	if err != nil {
		panic(err)
	}
	newfile, err := os.Create("test100new")
	if err != nil {
		panic(err)
	}
	content := string(contentByte)
	lines := strings.Split(content, "\n")

	// 从这里开始计时
	//start := time.Now()

	str := "100\n"
	for _, line := range lines {
		for index, ch := range line {
			str += string(ch)
			if (index + 1) % 9 == 0 {
				str += "\n"
			}
		}
	}
	_, err = newfile.Write([]byte(str))
	if err != nil {
		panic(err)
	}
	defer newfile.Close()
}