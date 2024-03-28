### ASIAR

### Description

This repository contains a C program designed for network analysis, specifically for identifying subnets and resolving IP aliases within a given network. The program utilizes various data structures and algorithms to process network trace files, identify IP address patterns, and segregate these IPs into subnets while also attempting to resolve any aliasing of IP addresses that occur when multiple IP addresses map to the same physical or logical network interface.

### Components

- **main.c**: The main executable file of the tool, responsible for parsing command-line arguments, initializing data structures, and orchestrating the flow of the subnet identification and alias resolution process.

- **libraries**:
  - **linkedlist_alias.h**: Defines structures and functions for handling linked lists used in alias resolution.
  - **asiarlib.h**: A library that provides functions for subnet analysis and IP address manipulation.
  - **binarytree_seen.h**: Implements binary tree data structures for efficiently storing and querying IP addresses and their attributes during analysis.

- **Parameters**: The program accepts various command-line arguments to specify input files, output files, and operation modes. These include paths to trace files, ground truth IP lists, subnet and alias output files, and configuration parameters for analysis thresholds and modes.

- **Data Structures**:
  - **binarytree_ip**: Used for storing and organizing IP addresses in a binary tree structure for efficient access and manipulation.
  - **binarytree_unresponsive**: A specialized binary tree for tracking unresponsive IP addresses during the subnet scanning process.
  - **subnet**: Represents a subnet, including information about its IPs, mask, and completeness.

- **Analysis Flow**:
  1. **Initialization**: Sets up necessary data structures and parses command-line arguments.
  2. **Reading Input**: Loads trace files, ground truth IPs, and other inputs as specified.
  3. **Subnet Identification**: Utilizes a combination of binary trees and linked lists to organize IP addresses, identify potential subnets, and evaluate their validity based on the provided traces and additional criteria.
  4. **Alias Resolution**: Attempts to resolve IP aliases by analyzing connectivity patterns and applying predefined rules and heuristics.
  5. **Output Generation**: Writes the analysis results to specified output files, including identified subnets and resolved aliases.

- **Utilities**:
  - Functions for IP address conversion, tree and list manipulation, and analysis criteria checks are provided to support the main functionality.

### Prerequisites

- A C compiler (e.g., GCC) to compile the program.
- Standard C libraries.
- Input trace files and any additional data files specified through command-line arguments.

### Installation

1. Clone the repository to your local machine.
2. Navigate to the repository directory.
3. Compile the program using a C compiler:
   ```sh
   gcc -o subnet_analysis main.c -I./libraries
   ```
4. Run the compiled executable with the necessary command-line arguments.

### Usage

```sh
./subnet_analysis [options]
```

Options include input file paths (`-i` for trace files, `-gi` for ground truth IPs), output file paths (`-so` for subnet output, `-ao` for alias output), and analysis parameters (`-ct` for completeness threshold, `-ss` for stack size).

### Contributing

Contributions are welcome. Please open issues for bugs and feature requests and submit pull requests for code changes.

### License

Specify the license under which this tool is released, typically included as a separate LICENSE file in the repository.

This README provides a general overview and setup guide for the tool. For detailed documentation on each component and function, refer to the comments and documentation within the source code files.
