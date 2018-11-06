done = false
Thread.new{	
	`g++ -std=c++17 main.cpp -lglut -lGLU -lGL -lpng -lstdc++fs`
	done = true
}

(sleep(0.1) and print '.') until done
puts "\nDone!"