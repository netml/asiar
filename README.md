### ASIAR

### Description

This repository contains a C program designed for network analysis, specifically for identifying subnets and resolving IP aliases within a given network. The program utilizes various data structures and algorithms to process network trace files, identify IP address patterns, and segregate these IPs into subnets while also attempting to resolve any aliasing of IP addresses that occur when multiple IP addresses map to the same physical or logical network interface.

### Components

- **main.c**: The main executable file of the tool, responsible for parsing command-line arguments, initializing data structures, and orchestrating the flow of the subnet identification and alias resolution process.

- **libraries**:
  - **binarytree.h/c**: Implements general binary tree data structures for storing and querying data efficiently.
  - **binarytree_seen.h/c**: Specializes in tracking seen IP addresses or entities, using binary tree structures to optimize search and storage.
  - **binarytree_ip.h/c**: Designed for managing IP addresses within a binary tree, facilitating operations specific to network address management.
  - **binarytree_alias.h/c**: Utilizes binary trees for alias resolution, aiding in the identification and mapping of network device aliases.
  - **binarytree_subnet.h/c**: Handles subnet information in a binary tree format, supporting subnet analysis and management.
  - **binarytree_unresponsive.h/c**: Tracks unresponsive nodes or addresses, employing binary trees to enhance network diagnostics.
  - **binarytree_duple.h/c** and **binarytree_duple_unclassified.h/c**: Focus on storing pairs of data, useful for managing correlated attributes or classifications.
  - **binarytree2.h/c**: Represents an advanced or alternative version of the binary tree library, offering additional features or improved performance.
  - **linkedlist.h/c**: Provides foundational structures and functions for linked list operations, suitable for a variety of data management tasks.
  - **linkedlist_alias.h/c**: Dedicated to managing alias links within linked lists, facilitating efficient alias resolution processes.
  - **linkedlist_distance.h/c**: Targets the storage and calculation of distances or metrics, potentially applicable in network topology analysis.
  - **network.h/c**: Offers a suite of functions for comprehensive network operations, including data transmission and network configuration utilities.
  - **asiarlib.h/c**: Specializes in subnet analysis and IP address manipulation, supporting intricate network analysis tasks.
  - **clib.h/c**: Contains a collection of general-purpose utilities and functions, applicable across various components of the project for basic data handling.

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

This README provides a general overview and setup guide for the tool. For detailed documentation on each component and function, refer to the comments and documentation within the source code files.
