#include "../libraries/linkedlist_alias.h"
#include "../libraries/asiarlib.h"
#include "../libraries/binarytree_seen.h"
#include <sys/resource.h>

int main(int argc, char const *argv[]) {
    ////////////////
    // Parameters //
    ////////////////
    binarytree_ip* ground_truth_ips = NULL; // this contains ground truth ips in case they were wanted to be removed from the traces
    binarytree_unresponsive* unresponsive = NULL;
    char const* filename = NULL; // the path of the traces file
    char const* ground_truth_ips_filename = NULL; // the path of the ground truth ips
    char const* modified_traces_filename = "kapar_out.kt"; // the path of the file where the subnets are to be saved
    char const* subnet_output_filename = "subnets.txt"; // the path of the file where the subnets are to be saved
    char const* subnet_edge_output_filename = "subnets_edges.txt"; // the path of the file where the subnets are to be saved
    char const* subnet_output_filename_detailed = "subnets_detailed.txt"; // the path of the file where the subnets with the IPs they contain are to be saved
    char const* duple_output_filename = "duples.txt"; // the path of the file where the subnets with the IPs they contain are to be saved
    char const* alias_output_filename = "aliases"; // the path of the file where the aliases are to be saved
    char const* log_output_filename = "log.txt"; // the path of the file where the logs are to be saved
    char const* announcement_filename = NULL; // the path of the announcements file
    char* AS_number = NULL; // holds the AS number
    char const* midar_filename = NULL;
    bool verbose = false; // for printing the detailed output
    bool tmode = false; // used for testing
    bool only_pair_aliases = true;
    int announcements_size = 0;
    mask* announcements_array = NULL;
    FILE *logs_outfile = NULL;
    FILE *subnets_outfile = NULL;
    FILE *subnets_detailed_outfile = NULL;
    FILE *subnets_edge_outfile = NULL;
    //FILE **aliases_outfile = NULL;
    FILE *aliases_outfile = NULL;
    FILE *midar_infile = NULL;
    int size_vps_ingress = 0;
    int size_vps_egress = 0;
    int size_vps_vp = 0;
    int size_ips_t = 0;
    int stack_size = 32;
    uint32_t *vps_ingress_array = NULL;
    uint32_t *vps_egress_array = NULL;
    uint32_t *vps_vp_array = NULL;
    uint32_t *ips_t_array = NULL;
    int no_of_non_zero_ips = 0;
    short **ingress = NULL;
    short **egress = NULL;
    short **vps = NULL;
    int *ips_subnet_id_array = NULL; // this array will hold the IDs of the subnets that each IP belongs to (size: size_ips_t)
    separators_struct* separators = NULL;
    int separators_size = 0;
    int subnets_size = 0;
    subnet* subnets_array = NULL; // holds the ips and their masks (as array)
    binarytree_ip **conflict_set_subnet = NULL;
    binarytree_ip **conflict_set = NULL;
    linkedlist **neighbors = NULL;
    int *conflict_set_positions = NULL;
    int *conflict_set_subnet_positions = NULL;
    bool subnet_only = false;
    int subnet_id = 1; // offset for subnet numbering
    int *alias_checks = (int*) malloc(sizeof(int) * 27);
    bool write_every_step = false;
    bool write_traces = false;

    int mode = 1; // 1 distance, 2 conflict set, 3 both
    int no_of_steps = 1;
    double completeness_threshold = 0.5;
    double subnet_pair_error_threshold = 0.0;
    double subnet_group_error_threshold = 0.0;

    bool is_set_mode = false;
    bool is_no_of_steps = false;
    bool is_alias_checks = false;
    bool is_set_completeness_threshold = false;
    bool is_set_subnet_pair_error_threshold = false;
    bool is_set_subnet_group_error_threshold = false;
    /////////
    // END //
    /////////


    /////////////////////////////////////
    // Read parameters from user input //
    /////////////////////////////////////
    // if no parameters are provided, print help and exit
    if (argc < 2) {
        help();
        exit(1);
    }

    // for each parameter, get and set the parameters
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
            filename = argv[i+1];
            //AS_number = getFileNameFromPath(argv[i+1]); // AS number
            i++;
        }
        else if (strcmp(argv[i], "-op") == 0 || strcmp(argv[i], "--only-pairs") == 0) {
            only_pair_aliases = false;
        }
        else if (strcmp(argv[i], "-os") == 0 || strcmp(argv[i], "--only-subnet") == 0) {
            subnet_only = true;
        }
        else if (strcmp(argv[i], "-so") == 0 || strcmp(argv[i], "--subnet-output") == 0) {
            subnet_output_filename = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-lo") == 0 || strcmp(argv[i], "--log-output") == 0) {
            log_output_filename = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-sod") == 0 || strcmp(argv[i], "--subnet-output-detailed") == 0) {
            subnet_output_filename_detailed = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-soe") == 0 || strcmp(argv[i], "--subnet-output-edges") == 0) {
            subnet_edge_output_filename = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-ap") == 0 || strcmp(argv[i], "--alias-parameters") == 0) {
            int pos = 0;
            char *pt;

            pt = strtok ((char*)argv[i+1], ",");
            while (pt != NULL) {
                if (pos > 27) {
                    printf("ERROR! Out of array!\n");
                    exit(0);
                }

                int a = atoi(pt);
                alias_checks[pos] = a;
                pos++;
                pt = strtok (NULL, ",");
            }

            if (!is_no_of_steps)
                no_of_steps = pos;

            if (no_of_steps > pos) {
                printf("ERR!\n");
                exit(0);
            }

            is_alias_checks = true;
            i++;
        }
        else if (strcmp(argv[i], "-ns") == 0 || strcmp(argv[i], "--no-of-steps") == 0) {
            if (no_of_steps < 1 || no_of_steps > 13) {
                printf("ERROR! The number of steps need to be between 1-13!\n");
                exit(1);
            }

            no_of_steps = atof(argv[i+1]);
            is_no_of_steps = true;
            i++;
        }
        else if (strcmp(argv[i], "-mi") == 0 || strcmp(argv[i], "--midar-input") == 0) {
            midar_filename = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-gi") == 0 || strcmp(argv[i], "--ground-truth-input") == 0) {
            ground_truth_ips_filename = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-mt") == 0 || strcmp(argv[i], "--modified-traces") == 0) {
            modified_traces_filename = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-do") == 0 || strcmp(argv[i], "--duples-output") == 0) {
            duple_output_filename = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-ao") == 0 || strcmp(argv[i], "--alias-output") == 0) {
            alias_output_filename = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-ai") == 0 || strcmp(argv[i], "--announcement-input") == 0) {
            announcement_filename = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-as") == 0 || strcmp(argv[i], "--as-no") == 0) {
            AS_number = (char*) argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-ct") == 0 || strcmp(argv[i], "--completeness-threshold") == 0) {
            completeness_threshold = atof(argv[i+1]);
            is_set_completeness_threshold = true;
            i++;
        }
        else if (strcmp(argv[i], "-ss") == 0 || strcmp(argv[i], "--stack-size") == 0) {
            stack_size = atoi(argv[i+1]);
            i++;
        }
        else if (strcmp(argv[i], "-sp") == 0 || strcmp(argv[i], "--subnet-pair-error") == 0) {
            subnet_pair_error_threshold = atof(argv[i+1]);
            is_set_subnet_pair_error_threshold = true;
            i++;
        }
        else if (strcmp(argv[i], "-sg") == 0 || strcmp(argv[i], "--subnet-group-error") == 0) {
            subnet_group_error_threshold = atof(argv[i+1]);
            is_set_subnet_group_error_threshold = true;
            i++;
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        }
        else if (strcmp(argv[i], "-ws") == 0 || strcmp(argv[i], "--write-every-step") == 0) {
            write_every_step = true;
        }
        else if (strcmp(argv[i], "-wt") == 0 || strcmp(argv[i], "--write-traces") == 0) {
            write_traces = true;
        }
        else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--mode") == 0) {
            mode = atoi(argv[i+1]);
            is_set_mode = true;
            i++;
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help();
            exit(1);
        }
        else {
            printf("ERROR! Unknown parameter provided! %s\n", argv[i]);
            exit(1);
        }
    }
    /////////
    // END //
    /////////


    // Set stack size
    const rlim_t kStackSize = stack_size * 1024 * 1024;
    struct rlimit rl;
    int result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0) {
        if (rl.rlim_cur < kStackSize) {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
                fprintf(stderr, "setrlimit returned result = %d\n", result);
        }
    }
    //printf ("Stack Limit = %ld and %ld max\n", rl.rlim_cur, rl.rlim_max);


    // open stream for the log file
    logs_outfile = fopen(log_output_filename, "w"); // open the file
    if (logs_outfile == NULL) {
        printf("ERROR! Log file pointer is null.\n");
        fprintf(logs_outfile, "ERROR! Log file pointer is null.\n");
        exit(1);
    }

    ///////////////////////
    // VALIDATION CHECKS //
    ///////////////////////
    // check if input file is provided, if not exit
    if (filename == NULL) {
        printf("ERROR! Traces file is not provided! Use -i to set the path to the traces file.\n");
        fprintf(logs_outfile, "ERROR! Traces file is not provided! Use -i to set the path to the traces file.\n");
        exit(1);
    }

    // check if announcements file is provided, if not enter testing mode
    if (announcement_filename == NULL) {
        printf("WARNING! Announcement file not provided! Entering the testing mode.\nUse -ai if you wish to provide the path to the announcements file.\n\n");
        fprintf(logs_outfile, "WARNING! Announcement file not provided! Entering the testing mode.\nUse -ai if you wish to provide the path to the announcements file.\n\n");
        tmode = true;
    }
    /////////
    // END //
    /////////

    ///////////////////////////////////////////////////////
    /////////////////////// SUBNETS ///////////////////////
    ///////////////////////////////////////////////////////


    ///////////////////////////////////////////////////
    // Read ground truth ips if the file is provided //
    ///////////////////////////////////////////////////
    if (ground_truth_ips_filename != NULL) // if file is provided
        ground_truth_ips = get_ground_truth_ips(ground_truth_ips_filename);

    printf("Number of ips in the ground truth: %d\n", size_tree_ip(ground_truth_ips));

    /////////////////
    // Build masks //
    /////////////////
    uint32_t *mask_zero = (uint32_t*) malloc(sizeof(uint32_t) * 33);
    uint32_t *mask_one = (uint32_t*) malloc(sizeof(uint32_t) * 33);
    build_masks_ones(mask_one);
    build_masks_zero(mask_zero);
    /////////
    // END //
    /////////


    //////////////////////////////////////////
    // Get announcements for the current AS //
    //////////////////////////////////////////
    if (!tmode) {
        printf("Getting the annoucements for the AS...");
        fprintf(logs_outfile, "Getting the annoucements for the AS...\n");

        binarytree* announcements = NULL;
        announcements = get_announcements(announcements, AS_number, announcement_filename); // holds ips and their masks

        if (announcements == NULL) {
            printf("ERROR! No announcements were read! Make sure the annoucements file is valid.\n");
            fprintf(logs_outfile, "ERROR! No announcements were read! Make sure the annoucements file is valid.\n");
            exit(1);
        }

        // convert announcements tree to array
        announcements_size = size_tree(announcements); // number of announcements for the current AS
        announcements_array = (mask*) malloc(sizeof(mask) * announcements_size); // holds the ips and their masks (as array)
        int pos = 0; // position to add to the array
        convert_tree_to_array_mask(announcements, announcements_array, &pos); // convert tree to array
        delete_tree(announcements); // delete the tree

        printf("\n");
    }
    /////////
    // END //
    /////////

    // find the number of traces
    int no_of_traces = 0;
    no_of_traces = get_no_of_traces_in_file(filename);
    trace* traces = (trace*) malloc(sizeof(trace) * no_of_traces);

    //////////////////////////////////////////////////////////
    // Get IPs, VPs, Ingresses and Egresses and their sizes //
    //////////////////////////////////////////////////////////
    binarytree_ip* vps_ingress = NULL;
    binarytree_ip* vps_egress = NULL;
    binarytree_ip* vps_vp = NULL;
    binarytree_ip* ips_t = NULL;
    binarytree_ip* ips_traced = NULL; // the ips that are seen at the end of traces

    printf("Reading IPs from the traces file...\n");
    fprintf(logs_outfile, "Reading IPs from the traces file...\n");

    // get IPs, VPs, Ingresses and Egresses
    if (verbose)
        printf("Traces as they were read by the get_ips_vps function\n");
    get_ips_vps(traces, &unresponsive, filename, &vps_ingress, &vps_egress, &vps_vp, &ips_t, &ips_traced, announcements_array, announcements_size, mask_one, verbose, tmode, no_of_traces, ground_truth_ips, modified_traces_filename, write_traces, midar_filename);

    int no_of_ips_traced = size_tree_ip(ips_traced);
    delete_tree_ip(ips_traced);
    /////////
    // END //
    /////////


    /////////////////////////////
    // Convert trees to arrays //
    /////////////////////////////
    size_vps_ingress = size_tree_ip(vps_ingress);
    size_vps_egress = size_tree_ip(vps_egress);
    size_vps_vp = size_tree_ip(vps_vp);
    size_ips_t = size_tree_ip(ips_t);

    if (size_ips_t == 0) {
        printf("ERROR! No IPs were read!\n");
        fprintf(logs_outfile, "ERROR! No IPs were read!\n");
        exit(1);
    }

    if (size_vps_ingress == 0 && size_vps_vp == 0) {
        printf("ERROR! No ingresses or egresses were read!\n");
        fprintf(logs_outfile, "ERROR! No ingresses or egresses were read!\n");
        exit(1);
    }

    vps_ingress_array = (uint32_t*) malloc(sizeof(uint32_t) * size_vps_ingress);
    vps_egress_array = (uint32_t*) malloc(sizeof(uint32_t) * size_vps_egress);
    vps_vp_array = (uint32_t*) malloc(sizeof(uint32_t) * size_vps_vp);
    ips_t_array = (uint32_t*) malloc(sizeof(uint32_t) * size_ips_t);

    int pos = 0;
    convert_tree_to_array_ip(vps_ingress, vps_ingress_array, &pos); // convert tree to array
    pos = 0;
    convert_tree_to_array_ip(vps_egress, vps_egress_array, &pos); // convert tree to array
    pos = 0;
    convert_tree_to_array_ip(vps_vp, vps_vp_array, &pos); // convert tree to array
    pos = 0;
    convert_tree_to_array_ip(ips_t, ips_t_array, &pos); // convert tree to array

    // delete trees
    delete_tree_ip(vps_ingress);
    delete_tree_ip(vps_egress);
    delete_tree_ip(vps_vp);
    delete_tree_ip(ips_t);
    /////////
    // END //
    /////////


    // get the number of non-zero ips
    for (int i = 0; i < size_ips_t; i++)
        if (!ip_is_zero(ips_t_array[i]))
            no_of_non_zero_ips++;


    ///////////////////////////
    // Print info about data //
    ///////////////////////////
    printf("\n");
    fprintf(logs_outfile, "\n");

    printf("No. of IPs:          %d\n", size_ips_t);
    fprintf(logs_outfile, "No. of IPs:          %d\n", size_ips_t);
    printf("No. of non-zero IPs: %d\n", no_of_non_zero_ips);
    fprintf(logs_outfile, "No. of non-zero IPs: %d\n", no_of_non_zero_ips);
    printf("No. of zero IPs:     %d\n", (size_ips_t - no_of_non_zero_ips));
    fprintf(logs_outfile, "No. of zero IPs:     %d\n", (size_ips_t - no_of_non_zero_ips));
    printf("No. of VPs:          %d\n", size_vps_vp);
    fprintf(logs_outfile, "No. of VPs:          %d\n", size_vps_vp);
    printf("No. of Ingresses:    %d\n", size_vps_ingress);
    fprintf(logs_outfile, "No. of Ingresses:    %d\n", size_vps_ingress);
    printf("No. of Egresses:     %d\n", size_vps_egress);
    fprintf(logs_outfile, "No. of Egresses:     %d\n", size_vps_egress);
    printf("No. of IPs traced:   %d\n\n", no_of_ips_traced);
    fprintf(logs_outfile, "No. of IPs traced:   %d\n\n", no_of_ips_traced);
    /////////
    // END //
    /////////


    ////////////////////
    // Set parameters //
    ////////////////////
    // if user did not provide any parameters
    double dest_rate = (double) no_of_ips_traced / (double) no_of_non_zero_ips;

    if ((size_vps_ingress + size_vps_vp) <= 3) { // 1x?
        if (!is_set_mode)
            mode = 1;

        if (!is_set_subnet_pair_error_threshold)
            subnet_pair_error_threshold = 0.0;

        if (!is_set_subnet_group_error_threshold)
            subnet_group_error_threshold = 0.0;

        if (dest_rate <= 0.69) { // 1x100 (copied from 1x1000)
            printf("Using the parameters estimated for 1x100 network...\n");

            if (!is_set_completeness_threshold)
                completeness_threshold = 0.8;

            if (!is_alias_checks) {
                alias_checks = (int*) malloc(sizeof(int) * 13);
                alias_checks[0] = 9;
                alias_checks[1] = 10;
                alias_checks[2] = 1;
                alias_checks[3] = 2;
                alias_checks[4] = 6;

                alias_checks[5] = 11;
                alias_checks[6] = 12;
                alias_checks[7] = 4;
                alias_checks[8] = 5;
                alias_checks[9] = 7;
                alias_checks[10] = 30;
                alias_checks[11] = 32;
                alias_checks[12] = 31;

                if (!is_no_of_steps)
                    no_of_steps = 5;

                if (no_of_steps > 13) {
                    printf("ER!\n");
                    exit(0);
                }
            }
        }
        else if (dest_rate <= 0.94) { // 1x1000
            printf("Using the parameters estimated for 1x1000 network...\n");

            if (!is_set_completeness_threshold)
                completeness_threshold = 0.6;

            if (!is_alias_checks) {
                alias_checks = (int*) malloc(sizeof(int) * 13);
                alias_checks[0] = 9;
                alias_checks[1] = 10;
                alias_checks[2] = 1;
                alias_checks[3] = 2;
                alias_checks[4] = 6;

                alias_checks[5] = 11;
                alias_checks[6] = 12;
                alias_checks[7] = 4;
                alias_checks[8] = 5;
                alias_checks[9] = 7;
                alias_checks[10] = 30;
                alias_checks[11] = 32;
                alias_checks[12] = 31;

                if (!is_no_of_steps)
                    no_of_steps = 5;

                if (no_of_steps > 13) {
                    printf("ER!\n");
                    exit(0);
                }
            }
        }
        else { // 1xall
            printf("Using the parameters estimated for 1xall network...\n");

            if (!is_set_completeness_threshold)
                completeness_threshold = 0.4;

            if (!is_alias_checks) {
                alias_checks = (int*) malloc(sizeof(int) * 13);
                alias_checks[0] = 9;
                alias_checks[1] = 11;
                alias_checks[2] = 10;
                alias_checks[3] = 12;
                alias_checks[4] = 1;
                alias_checks[5] = 4;
                alias_checks[6] = 2;
                alias_checks[7] = 5;
                alias_checks[8] = 7;

                alias_checks[9] = 6;
                alias_checks[10] = 30;
                alias_checks[11] = 32;
                alias_checks[12] = 31;

                if (!is_no_of_steps)
                    no_of_steps = 9;

                if (no_of_steps > 13) {
                    printf("ER!\n");
                    exit(0);
                }
            }
        }
    }
    else if ((size_vps_ingress + size_vps_vp) <= 30) { // 10x?
        if (!is_set_mode)
            mode = 1;

        if (!is_set_subnet_pair_error_threshold)
            subnet_pair_error_threshold = 0.0;

        if (!is_set_subnet_group_error_threshold)
            subnet_group_error_threshold = 0.0;

        if (dest_rate <= 0.4) { // 10x100
            printf("Using the parameters estimated for 10x100 network...\n");

            if (!is_set_completeness_threshold)
                completeness_threshold = 0.8;

            if (!is_alias_checks) {
                alias_checks = (int*) malloc(sizeof(int) * 13);
                alias_checks[0] = 6;

                alias_checks[1] = 7;
                alias_checks[2] = 31;
                alias_checks[3] = 30;
                alias_checks[4] = 32;
                alias_checks[5] = 5;
                alias_checks[6] = 9;
                alias_checks[7] = 11;
                alias_checks[8] = 10;
                alias_checks[9] = 12;
                alias_checks[10] = 1;
                alias_checks[11] = 4;
                alias_checks[12] = 2;

                if (!is_no_of_steps)
                    no_of_steps = 1;

                if (no_of_steps > 13) {
                    printf("ER!\n");
                    exit(0);
                }
            }
        }
        else if (dest_rate <= 0.85) { // 10x1000
            printf("Using the parameters estimated for 10x1000 network...\n");

            if (!is_set_completeness_threshold)
                completeness_threshold = 0.6;

            if (!is_alias_checks) {
                alias_checks = (int*) malloc(sizeof(int) * 13);
                alias_checks[0] = 9;
                alias_checks[1] = 10;
                alias_checks[2] = 1;

                alias_checks[3] = 2;
                alias_checks[4] = 6;
                alias_checks[5] = 4;
                alias_checks[6] = 11;
                alias_checks[7] = 12;
                alias_checks[8] = 5;
                alias_checks[9] = 32;
                alias_checks[10] = 30;
                alias_checks[11] = 7;
                alias_checks[12] = 31;

                if (!is_no_of_steps)
                    no_of_steps = 3;

                if (no_of_steps > 13) {
                    printf("ER!\n");
                    exit(0);
                }
            }
        }
        else { // 10xall
            printf("Using the parameters estimated for 10xall network...\n");

            if (!is_set_completeness_threshold)
                completeness_threshold = 0.5;

            if (!is_alias_checks) {
                alias_checks = (int*) malloc(sizeof(int) * 13);
                alias_checks[0] = 9;
                alias_checks[1] = 10;
                alias_checks[2] = 1;
                alias_checks[3] = 2;

                alias_checks[4] = 30;
                alias_checks[5] = 32;
                alias_checks[6] = 11;
                alias_checks[7] = 12;
                alias_checks[8] = 4;
                alias_checks[9] = 5;
                alias_checks[10] = 6;
                alias_checks[11] = 31;
                alias_checks[12] = 7;

                if (!is_no_of_steps)
                    no_of_steps = 4;

                if (no_of_steps > 13) {
                    printf("ER!\n");
                    exit(0);
                }
            }
        }
    }
    else if ((size_vps_ingress + size_vps_vp) <= 300) { // 100x?
        if (!is_set_mode)
            mode = 1;

        if (!is_set_subnet_pair_error_threshold)
            subnet_pair_error_threshold = 0.0;

        if (!is_set_subnet_group_error_threshold)
            subnet_group_error_threshold = 0.0;

        if (dest_rate <= 0.16) { // 100x100
            printf("Using the parameters estimated for 100x100 network...\n");

            if (!is_set_completeness_threshold)
                completeness_threshold = 0.7;

            if (!is_alias_checks) {
                alias_checks = (int*) malloc(sizeof(int) * 13);
                alias_checks[0] = 6;

                alias_checks[1] = 31;
                alias_checks[2] = 7;
                alias_checks[3] = 4;
                alias_checks[4] = 30;
                alias_checks[5] = 32;
                alias_checks[6] = 5;
                alias_checks[7] = 11;
                alias_checks[8] = 12;
                alias_checks[9] = 9;
                alias_checks[10] = 10;
                alias_checks[11] = 1;
                alias_checks[12] = 2;

                if (!is_no_of_steps)
                    no_of_steps = 1;

                if (no_of_steps > 13) {
                    printf("ER!\n");
                    exit(0);
                }
            }
        }
        else if (dest_rate <= 0.67) { // 100x1000
            printf("Using the parameters estimated for 100x1000 network...\n");

            if (!is_set_completeness_threshold)
                completeness_threshold = 0.6;

            if (!is_alias_checks) {
                alias_checks = (int*) malloc(sizeof(int) * 13);
                alias_checks[0] = 9;
                alias_checks[1] = 10;
                alias_checks[2] = 1;
                alias_checks[3] = 2;

                alias_checks[4] = 32;
                alias_checks[5] = 30;
                alias_checks[6] = 11;
                alias_checks[7] = 12;
                alias_checks[8] = 4;
                alias_checks[9] = 5;
                alias_checks[10] = 6;
                alias_checks[11] = 7;
                alias_checks[12] = 31;

                if (!is_no_of_steps)
                    no_of_steps = 4;

                if (no_of_steps > 13) {
                    printf("ER!\n");
                    exit(0);
                }
            }
        }
        else { // 100xall
            printf("Using the parameters estimated for 100xall network...\n");

            if (!is_set_completeness_threshold)
                completeness_threshold = 0.8;

            if (!is_alias_checks) {
                alias_checks = (int*) malloc(sizeof(int) * 13);
                alias_checks[0] = 9;
                alias_checks[1] = 10;
                alias_checks[2] = 1;
                alias_checks[3] = 2;

                alias_checks[4] = 32;
                alias_checks[5] = 30;
                alias_checks[6] = 4;
                alias_checks[7] = 5;
                alias_checks[8] = 11;
                alias_checks[9] = 12;
                alias_checks[10] = 6;
                alias_checks[11] = 31;
                alias_checks[12] = 7;

                if (!is_no_of_steps)
                    no_of_steps = 4;

                if (no_of_steps > 13) {
                    printf("ER!\n");
                    exit(0);
                }
            }
        }
    }
    else { // 1000x?
        if (!is_set_mode)
            mode = 3;

        if (!is_set_subnet_pair_error_threshold)
            subnet_pair_error_threshold = 0.1;

        if (!is_set_subnet_group_error_threshold)
            subnet_group_error_threshold = 0.1;

        if (dest_rate <= 0.54) { // 1000x1000
            printf("Using the parameters estimated for 1000x1000 network...\n");

            if (!is_set_completeness_threshold)
                completeness_threshold = 0.9;

            if (!is_alias_checks) {
                alias_checks = (int*) malloc(sizeof(int) * 13);
                alias_checks[0] = 9;
                alias_checks[1] = 10;
                alias_checks[2] = 1;
                alias_checks[3] = 2;

                alias_checks[4] = 32;
                alias_checks[5] = 30;
                alias_checks[6] = 4;
                alias_checks[7] = 1;
                alias_checks[8] = 12;
                alias_checks[9] = 5;
                alias_checks[10] = 6;
                alias_checks[11] = 7;
                alias_checks[12] = 31;

                if (!is_no_of_steps)
                    no_of_steps = 4;

                if (no_of_steps > 13) {
                    printf("ER!\n");
                    exit(0);
                }
            }
        }
        else { // 1000xall (copied from 1000x1000)
            printf("Using the parameters estimated for 1000xall network...\n");

            if (!is_set_completeness_threshold)
                completeness_threshold = 0.99;

            if (!is_alias_checks) {
                alias_checks = (int*) malloc(sizeof(int) * 13);
                alias_checks[0] = 9;
                alias_checks[1] = 10;
                alias_checks[2] = 1;
                alias_checks[3] = 2;

                alias_checks[4] = 32;
                alias_checks[5] = 30;
                alias_checks[6] = 4;
                alias_checks[7] = 1;
                alias_checks[8] = 12;
                alias_checks[9] = 5;
                alias_checks[10] = 6;
                alias_checks[11] = 7;
                alias_checks[12] = 31;

                if (!is_no_of_steps)
                    no_of_steps = 4;

                if (no_of_steps > 13) {
                    printf("ER!\n");
                    exit(0);
                }
            }
        }
    }

    printf("Mode: ");
    if (mode == 1) {
        printf("distance (1)\n");
        fprintf(logs_outfile, "distance (1)\n");
    }
    else if (mode == 2) {
        printf("conflict (2)\n");
        fprintf(logs_outfile, "conflict (2)\n");
    }
    else if (mode == 3) {
        printf("distance & conflict (3)\n");
        fprintf(logs_outfile, "distance & conflict (3)\n");
    }

    printf("Completeness threshold: %f\n", completeness_threshold);
    fprintf(logs_outfile, "Completeness threshold: %f\n", completeness_threshold);

    printf("Pair-error threshold: %f\n", subnet_pair_error_threshold);
    fprintf(logs_outfile, "Pair-error threshold: %f\n", subnet_pair_error_threshold);

    printf("Group-error threshold: %f\n", subnet_group_error_threshold);
    fprintf(logs_outfile, "Group-error threshold: %f\n", subnet_group_error_threshold);
    /////////
    // END //
    /////////


    ////////////////////////////////////////////////////////////////////
    // Create the matrices for ingress, egress and vp (2 dimensional) //
    ////////////////////////////////////////////////////////////////////
    ingress = (short**) malloc(sizeof(short*) * size_vps_ingress);
	for (int q = 0; q < size_vps_ingress; q++) {
        ingress[q] = (short*) malloc(sizeof(short) * size_ips_t);

        // initialize
        for (int i = 0; i < size_ips_t; i++)
            ingress[q][i] = (short) 255;
    }

    egress = (short**) malloc(sizeof(short*) * size_vps_egress);
	for (int q = 0; q < size_vps_egress; q++) {
        egress[q] = (short*) malloc(sizeof(short) * size_ips_t);

        // initialize
        for (int i = 0; i < size_ips_t; i++)
            egress[q][i] = (short) 255;
    }

    vps = (short**) malloc(sizeof(short*) * size_vps_vp);
	for (int q = 0; q < size_vps_vp; q++) {
        vps[q] = (short*) malloc(sizeof(short) * size_ips_t);

        // initialize
        for (int i = 0; i < size_ips_t; i++)
            vps[q][i] = (short) 255;
    }
    /////////
    // END //
    /////////


    ////////////////////////////////////
    // Create the array of Subnet IDs //
    ////////////////////////////////////
    ips_subnet_id_array = (int*) malloc(sizeof(int) * size_ips_t);
    for (int q = 0; q < size_ips_t; q++)
        ips_subnet_id_array[q] = 0; // initialize to 0 (meaning the IP does not belong to a subnet)
    /////////
    // END //
    /////////


    //////////////////////////////////////////////
    // Create the array of BST for conflict set //
    //////////////////////////////////////////////
    conflict_set_subnet = (binarytree_ip**) malloc(sizeof(binarytree_ip*) * size_ips_t);
	for (int q = 0; q < size_ips_t; q++)
        conflict_set_subnet[q] = NULL; // initialize

    conflict_set = (binarytree_ip**) malloc(sizeof(binarytree_ip*) * size_ips_t);
	for (int q = 0; q < size_ips_t; q++)
        conflict_set[q] = NULL; // initialize

    // this array will hold the positions of conflict sets for each IP
    conflict_set_positions = (int*) malloc(sizeof(int) * size_ips_t);
    for (int q = 0; q < size_ips_t; q++)
        conflict_set_positions[q] = q; // initialize to actual IP positions

    conflict_set_subnet_positions = (int*) malloc(sizeof(int) * size_ips_t);
    for (int q = 0; q < size_ips_t; q++)
        conflict_set_subnet_positions[q] = q; // initialize to actual IP positions
    /////////
    // END //
    /////////


    ////////////////////////////////////////
    // Create the linkedlist of neighbors //
    ////////////////////////////////////////
    neighbors = (linkedlist**) malloc(sizeof(linkedlist*) * size_ips_t);
    for (int i = 0; i < size_ips_t; i++)
        neighbors[i] = NULL;
    /////////
    // END //
    /////////


    ///////////////////////////////////////////////////////////////
    // Fill in the distances in matrices & populate conflict set //
    ///////////////////////////////////////////////////////////////
    printf("Determining the distances...\n");
    fprintf(logs_outfile, "Determining the distances...\n");

    if (verbose)
        printf("Traces as they were read by the get_ips_vps function\n");

    fill_distances(traces, &unresponsive, conflict_set_subnet, conflict_set, filename, ips_t_array, size_ips_t, announcements_array, announcements_size, mask_one, vps_vp_array, size_vps_vp, vps, vps_ingress_array, size_vps_ingress, ingress, vps_egress_array, size_vps_egress, egress, tmode, verbose, neighbors);

    printf("\n");
    fprintf(logs_outfile, "\n");
    /////////
    // END //
    /////////


    ///////////
    // Print //
    ///////////
    if (verbose) {
        printf("===========================================================================\n");
        printf("================================== VPS ====================================\n");
        printf("===========================================================================\n");

        for (int i = 0; i < size_vps_vp; i++)
            printf("VP: %s\n", convert_int_to_ip(vps_vp_array[i]));

        printf("\n");

        printf("===========================================================================\n");
        printf("============================== INGRESSES ==================================\n");
        printf("===========================================================================\n");

        for (int i = 0; i < size_vps_ingress; i++)
            printf("Ingress: %s\n", convert_int_to_ip(vps_ingress_array[i]));

        printf("\n");

        printf("===========================================================================\n");
        printf("================================== IPS ====================================\n");
        printf("===========================================================================\n");

        for (int i = 0; i < size_ips_t; i++)
            printf("IP: %s\n", convert_int_to_ip(ips_t_array[i]));

        printf("\n");

        printf("===========================================================================\n");
        printf("================================== TRACES =================================\n");
        printf("===========================================================================\n");

        for (int i = 0; i < size_vps_ingress; i++)
            for (int j = 0; j < size_ips_t; j++)
                if (ingress[i][j] != 255)
                    printf("INGRESS: %s, IP: %s, HOP: %d\n", convert_int_to_ip(vps_ingress_array[i]), convert_int_to_ip(ips_t_array[j]), ingress[i][j]);

        for (int i = 0; i < size_vps_egress; i++)
            for (int j = 0; j < size_ips_t; j++)
                if (egress[i][j] != 255)
                    printf("EGRESS: %s, IP: %s, HOP: %d\n", convert_int_to_ip(vps_egress_array[i]), convert_int_to_ip(ips_t_array[j]), egress[i][j]);

        for (int i = 0; i < size_vps_vp; i++)
            for (int j = 0; j < size_ips_t; j++)
                if (vps[i][j] != 255)
                    printf("VPS: %s, IP: %s, HOP: %d\n", convert_int_to_ip(vps_vp_array[i]), convert_int_to_ip(ips_t_array[j]), vps[i][j]);

        printf("\n");

        printf("===========================================================================\n");
        printf("================================= DISTANCES ===============================\n");
        printf("===========================================================================\n");
        printf("\n");

        printf("INGRESS\n");
        for (int j = 0; j < size_ips_t; j++) {
            int length = strlen(convert_int_to_ip(ips_t_array[j]));

            printf("%d: %s", j, convert_int_to_ip(ips_t_array[j]));
            for (int i = 0; i < (15 - length) + 5; i++)
                printf(" ");
            printf("|");

            for (int i = 0; i < size_vps_ingress; i++) {
                if (ingress[i][j] != 255)
                    printf("%d ", ingress[i][j]);
                else
                    printf("%s ", " ");
                printf("|");
            }

            printf("\n");
        }
        printf("\n");

        printf("VP\n");
        for (int j = 0; j < size_ips_t; j++) {
            int length = strlen(convert_int_to_ip(ips_t_array[j]));

            printf("%d: %s", j, convert_int_to_ip(ips_t_array[j]));
            for (int i = 0; i < (15 - length) + 5; i++)
                printf(" ");
            printf("|");

            for (int i = 0; i < size_vps_vp; i++) {
                if (vps[i][j] != 255)
                    printf("%d ", vps[i][j]);
                else
                    printf("%s ", " ");
                printf("|");
            }

            printf("\n");
        }
        printf("\n");

        printf("EGRESS\n");
        for (int j = 0; j < size_ips_t; j++) {
            int length = strlen(convert_int_to_ip(ips_t_array[j]));

            printf("%d: %s", j, convert_int_to_ip(ips_t_array[j]));
            for (int i = 0; i < (15 - length) + 5; i++)
                printf(" ");
            printf("|");

            for (int i = 0; i < size_vps_egress; i++) {
                if (egress[i][j] != 255)
                    printf("%d ", egress[i][j]);
                else
                    printf("%s ", " ");
                printf("|");
            }

            printf("\n");
        }

        printf("\n");

        printf("===========================================================================\n");
        printf("=============================== CONFLICT SETS =============================\n");
        printf("===========================================================================\n");
        printf("\n");

        printf("Conflict set for subnet resolution...\n");
        for (int j = 0; j < size_ips_t; j++) {

            printf("%s\n", convert_int_to_ip(ips_t_array[j]));

            printf("\t");
            print_inorder_ip(conflict_set_subnet[j]);

            printf("\n===============================\n");
        }
        printf("\n");

        printf("Conflict set for alias resolution...\n");
        for (int j = 0; j < size_ips_t; j++) {

            printf("%s\n", convert_int_to_ip(ips_t_array[j]));

            printf("\t");
            print_inorder_ip(conflict_set[j]);

            printf("\n===============================\n");
        }
        printf("\n");


        printf("neighbors\n");
        for (int i = 0; i < size_ips_t; i++) {
            printf("%s -> ", convert_int_to_ip(ips_t_array[i]));
            print_list2(neighbors[i]);
            printf("\n");
        }
        printf("\n");
    }
    /////////
    // END //
    /////////


    if (verbose) {
        printf("===========================================================================\n");
        printf("============================= FINDING SUBNETS =============================\n");
        printf("===========================================================================\n");
        printf("\n");
    }

    printf("Finding subnets...\n");
    fprintf(logs_outfile, "Finding subnets...\n");


    ////////////////////////
    // Check pair matches //
    ////////////////////////
    // determine pairs which fit the +/- 1 rule OR which do not conflict with the conflict set of IPs
    printf("Checking pairs...\n\n");

    separators_size = size_ips_t - 1;
    separators = (separators_struct*) malloc(sizeof(separators_struct) * separators_size); // holds whether ip pairs satisfy +/- 1 rule (i false means i and i+1 do not match and vice versa)
    binarytree_subnet* subnets = NULL;

    determine_separators(mode, separators_size, ips_t_array, separators, mask_one, mask_zero, size_vps_vp, vps, size_vps_ingress, ingress, size_vps_egress, egress, subnet_pair_error_threshold, conflict_set_subnet, conflict_set_subnet_positions, size_ips_t);
    /////////
    // END //
    /////////


    ////////////////////////////
    // Print after pair check //
    ////////////////////////////
    if (verbose) {
        printf("======================\n");
        printf("SEPARATORS FOR SUBNETS (after pair distance)\n");
        printf("======================\n\n");
        print_separators(separators_size, separators, ips_t_array, mask_one, mask_zero, size_ips_t);
        printf("\n");
    }
    /////////
    // END //
    /////////


    /////////////////////////
    // Check group matches //
    /////////////////////////
    if (verbose)
        printf("Checking group distances...\n\n");

    compare_group_distances(conflict_set_subnet, conflict_set_subnet_positions, verbose, separators_size, separators, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, ips_t_array, mask_one, mask_zero, size_ips_t, subnet_group_error_threshold, mode);
    /////////
    // END //
    /////////


    /////////////////////////////
    // Print after group check //
    /////////////////////////////
    if (verbose) {
        printf("======================\n");
        printf("SEPARATORS FOR SUBNETS (after group distances)\n");
        printf("======================\n\n");
        print_separators(separators_size, separators, ips_t_array, mask_one, mask_zero, size_ips_t);
        printf("\n");
    }
    /////////
    // END //
    /////////


    ////////////////////////
    // Completeness check //
    ////////////////////////
    if (verbose)
        printf("Checking for completeness...\n\n");

    check_completeness(verbose, separators_size, separators, ips_t_array, mask_one, size_ips_t, completeness_threshold, mask_zero);

    // with completeness
    //check_completeness(verbose, separators_size, separators, ips_t_array, mask_one, size_ips_t, completeness_threshold, mask_zero, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp);
    /////////
    // END //
    /////////


    ////////////////////////////////////
    // Print after completeness check //
    ////////////////////////////////////
    if (verbose) {
        printf("======================\n");
        printf("SEPARATORS FOR SUBNETS (after completeness)\n");
        printf("======================\n\n");
        print_separators(separators_size, separators, ips_t_array, mask_one, mask_zero, size_ips_t);
        printf("\n");
    }
    /////////
    // END //
    /////////


    /////////////////////////////////////////////
    // Add subnets to array and write to files //
    /////////////////////////////////////////////
    int start = 0;
    int end = 0;

    // open streams
    subnets_detailed_outfile = fopen(subnet_output_filename_detailed, "w"); // open the file
    subnets_edge_outfile = fopen(subnet_edge_output_filename, "w"); // open the file
    subnets_outfile = fopen(subnet_output_filename, "w"); // open the file

    // check if streams have any errors
    if (subnets_detailed_outfile == NULL || subnets_edge_outfile == NULL || subnets_outfile == NULL) {
        printf("ERROR! Detailed subnets output file pointer is null.\n");
        fprintf(subnets_detailed_outfile, "ERROR! Detailed subnets output file pointer is null.\n");
        exit(1);
    }

    // for each potential subnet block
    while (start < separators_size) {
        if (separators[start].state) { // find the start of the subnet
            end = find_end(separators, separators_size, start); // find the end of the subnet

            if (verbose)
                printf("Block start: %d, end: %d\n", start, end);

            // get the mask
            int mask = check_special_ips(start, end, verbose, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, true);

            if (mask != -1) { // if splitting had not occurred
                if (verbose)
                    printf("\tMask between start and end is: %d\n", mask);

                int no_of_ips = (end - start) + 1;
                int total_ips_in_subnet = pow(2, (32 - mask)) - 2;

                // set subnet id
                for (int j = start; j <= end; j++)
                    ips_subnet_id_array[j] = subnet_id;
                subnet_id++;

                // get the prefix for the subnet
                uint32_t ip = ips_t_array[start] & (uintptr_t)(mask_one[mask]);

                // when giant component used, this function is used
                //double completeness = get_subnet_completeness(no_of_ips, total_ips_in_subnet, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, ips_t_array, verbose);

                // calculate subnet completeness
                double completeness = (double) no_of_ips / (double) total_ips_in_subnet;

                if (verbose)
                    printf("Subnet completeness is: %f\n\n\n", completeness);

                // add subnet to the tree
                subnets = insert_tree_subnet(subnets, ip, mask, no_of_ips, total_ips_in_subnet, completeness);

                // write to "subnets" and "subnets_detailed" files
                fprintf(subnets_detailed_outfile, "%s/%d ", convert_int_to_ip(ip), mask);
                fprintf(subnets_outfile, "%s/%d\n", convert_int_to_ip(ip), mask);

                // write IPs of the subnet to "subnets_detailed" file
                for (int i = start; i <= end; i++)
                    fprintf(subnets_detailed_outfile, "%s ", convert_int_to_ip(ips_t_array[i]));
                fprintf(subnets_detailed_outfile, "\n");

                // write to "subnets_edges" file
                for (int i = start; i < end; i++)
                    for (int j = i+1; j <= end; j++)
                        fprintf(subnets_edge_outfile, "%s\t%s\n", convert_int_to_ip(ips_t_array[i]), convert_int_to_ip(ips_t_array[j]));

                start = end+1; // skip the subnet since we're done with it
            }
            else if (verbose)
                printf("Splitting occured during special IP check, going back recursively...\n");
        }
        else
            start++;
    }

    // close the streams
    fclose(subnets_edge_outfile);
    fclose(subnets_detailed_outfile);
    fclose(subnets_outfile);
    /////////
    // END //
    /////////


    subnet_id--; // remove 1 which gives the exact number of available subnets


    /////////////////////////////////
    // Print info on subnets found //
    /////////////////////////////////
    if (verbose) {
        printf("\n");
        printf("===========================================================================\n");
        printf("========================= NO OF SUBNETS FOUND: %d =========================\n", subnet_id);
        printf("===========================================================================\n\n");

        printf("===========================================================================\n");
        printf("================== IPs with THEIR SUBNET IDs: Pairs (ALL) =================\n");
        printf("===========================================================================\n\n");

        for (int i = 0; i < size_ips_t; i++) // for each IP
            printf("%d: %s\t\tid: %d\n", i, convert_int_to_ip(ips_t_array[i]), ips_subnet_id_array[i]);
        printf("\n");

        printf("===========================================================================\n");
        printf("======== SEPARATORS FOR SUBNETS with THEIR SUBNET IDs: Pairs (ALL) ========\n");
        printf("===========================================================================\n\n");

        for (int i = 0; i <= separators_size; i++) { // for each pair
            //int mask = get_mask_matches(i, i+1, mask_one, ips_t_array);
            int mask = check_special_ips(i, i+1, false, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, false);

            if (separators[i].state) {
                printf("%d: true (%s)", i, convert_int_to_ip(ips_t_array[i]));
                printf("\t\t\tmask: %d", mask);
                printf("\tsubnet_id: %d", ips_subnet_id_array[i]);
                printf("\n");
            }
            else {
                 printf("%d: \tfalse (%s)", i, convert_int_to_ip(ips_t_array[i]));
                 printf("\t\tmask: %d", mask);
                 printf("\tsubnet_id: %d", ips_subnet_id_array[i]);
                 printf("\n");
             }
        }
        printf("\n");
    }
    /////////
    // END //
    /////////


    /////////////////////////////////////
    // Create subnet/ip relation array //
    /////////////////////////////////////
    // create an array of BST where the array will represent subnets and the trees will contain the IPs that belong to the subnets
    binarytree_ip **subnets_containing_ips = (binarytree_ip**) malloc(sizeof(binarytree_ip*) * subnet_id);
    for (int q = 0; q < subnet_id; q++)
        subnets_containing_ips[q] = NULL;

    // Fill the BST
    for (int i = 0; i < size_ips_t; i++)
        if (ips_subnet_id_array[i] > 0)
            subnets_containing_ips[ips_subnet_id_array[i]-1] = insert_tree_ip(subnets_containing_ips[ips_subnet_id_array[i]-1], ips_t_array[i]);

    // Convert BST into array
    uint32_t **subnets_containing_ips_array = (uint32_t**) malloc(sizeof(uint32_t*) * subnet_id);
    int *sizes = (int*) malloc(sizeof(int) * subnet_id);

    for (int q = 0; q < subnet_id; q++) {
        sizes[q] = size_tree_ip(subnets_containing_ips[q]);
        subnets_containing_ips_array[q] = (uint32_t*) malloc(sizeof(uint32_t) * sizes[q]);

        pos = 0;
        convert_tree_to_array_ip(subnets_containing_ips[q], subnets_containing_ips_array[q], &pos); // convert tree to array

        delete_tree_ip(subnets_containing_ips[q]);
    }
// FIX
//    delete_tree_ip(*subnets_containing_ips);
    /////////
    // END //
    /////////


    ///////////////////////////////////
    // Convert subnets tree to array //
    ///////////////////////////////////
    subnets_size = size_tree_subnet(subnets); // number of announcements for the current AS
    subnets_array = (subnet*) malloc(sizeof(subnet) * subnets_size); // holds the ips and their masks (as array)
    pos = 0; // position to add to the array
    convert_tree_to_array_subnet(subnets, subnets_array, &pos); // convert tree to array
    delete_tree_subnet(subnets); // delete the tree

/*
FIX don't know why this sorting was there in the first place
    subnet_completeness_mergesort(subnets_array, 0, subnets_size-1);
*/
    /////////
    // END //
    /////////


    printf("Subnets generated!\n");
    fprintf(logs_outfile, "Subnets generated!\n");
    printf("No. of subnets: %d\n\n", subnets_size);
    fprintf(logs_outfile, "No. of subnets: %d\n\n", subnets_size);




















    ///////////////////////////////////////////////////////
    /////////////////////// ALIASES ///////////////////////
    ///////////////////////////////////////////////////////
    free(vps_ingress_array);
    free(vps_egress_array);
    free(vps_vp_array);

    ///////////////////////
    // Create alias sets //
    ///////////////////////
    int* alias_pos = (int*) malloc(sizeof(int) * size_ips_t);
    linkedlist** alias_tree = (linkedlist**) malloc(sizeof(linkedlist*) * size_ips_t);
    for (int i = 0; i < size_ips_t; i++) {
        alias_pos[i] = i;
        alias_tree[i] = NULL;
        alias_tree[i] = insert_list_alias(alias_tree[i], ips_t_array[i], i);
    }
    /////////
    // END //
    /////////


    //////////////////////
    // Alias resolution //
    //////////////////////
    if (subnets_size > 0 && !subnet_only) { // if there exist subnets and not in subnet-only mode
        ///////////////////////////////////////////////////////////////////////////////
        // Generate the array containing the no. of reference points each IP is seen //
        ///////////////////////////////////////////////////////////////////////////////
        int* reference_point_array = (int*) malloc(sizeof(int) * size_ips_t);
        for (int i = 0; i < size_ips_t; i++) { // for each IP
            int no_of_reference_points = 0;

            for (int j = 0; j < size_vps_vp; j++)
                if (vps[j][i] != 255)
                    no_of_reference_points++;

            for (int j = 0; j < size_vps_ingress; j++)
                if (ingress[j][i] != 255)
                    no_of_reference_points++;

            for (int j = 0; j < size_vps_egress; j++)
                if (egress[j][i] != 255)
                    no_of_reference_points++;

            reference_point_array[i] = no_of_reference_points;
        }
        /////////
        // END //
        /////////


        ///////////////////////////////
        // Get duple IPs from traces //
        ///////////////////////////////
        printf("Finding aliases...\n");
        printf("Getting the duples...\n");
        binarytree_duple* duples = NULL;
        get_edges(traces, &unresponsive, filename, &duples, ips_t_array, size_ips_t, announcements_array, announcements_size, mask_one, verbose, tmode, subnets_array, subnets_size, ips_subnet_id_array, reference_point_array, duple_output_filename);

        free(announcements_array);
        delete_tree_unresponsive(unresponsive);
        free(traces);
        /////////
        // END //
        /////////


        //////////////////
        // Print duples //
        //////////////////
        if (verbose) {
            printf("=========================================================================\n");
            printf("============================== ALIAS DUPLES =============================\n");
            printf("=========================================================================\n\n");
            print_inorder_duple(duples);
            printf("\n");
        }
        /////////
        // END //
        /////////


        //////////////////////////////////
        // Convert duples tree to array //
        //////////////////////////////////
        int duplets_size = size_tree_duple(duples); // number of aliases
        duple* aliases_array = (duple*) malloc(sizeof(duple) * duplets_size); // holds the ips and their masks (as array)
        pos = 0; // position to add to the array
        convert_duple_tree_to_array(duples, aliases_array, &pos); // convert tree to array
        delete_tree_duple(duples); // delete the tree
        /////////
        // END //
        /////////


        ////////////////////////////////////////////////////////////////
        // Sort the duples by their reference points and completeness //
        ////////////////////////////////////////////////////////////////
        printf("Sorting the duples...\n");

        duple_completeness_mergesort(aliases_array, 0, duplets_size-1);
        duple_reference_mergesort(aliases_array, 0, duplets_size-1);
        /////////
        // END //
        /////////


        /////////////////////////
        // Print sorted duples //
        /////////////////////////
        if (verbose) {
            printf("=========================================================================\n");
            printf("============================= SORTED DUPLES =============================\n");
            printf("=========================================================================\n\n");
            for (int i = 0; i < duplets_size; i++)
                printf("%s, %s \t\t comp: %f ref: %f\n", convert_int_to_ip(aliases_array[i].data1), convert_int_to_ip(aliases_array[i].data2), aliases_array[i].completeness, aliases_array[i].reference);
            printf("\n");
        }
        /////////
        // END //
        /////////


        if (verbose) {
            printf("===========================================================================\n");
            printf("============================= FINDING ALIASES =============================\n");
            printf("===========================================================================\n");
            printf("\n");
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Create and Populate the array which contains IPs and the duples that each IP appear in //
        ////////////////////////////////////////////////////////////////////////////////////////////
        // Create
        linkedlist_alias** duples_subnet_connection = (linkedlist_alias**) malloc(sizeof(linkedlist_alias*) * size_ips_t);
        for (int i = 0; i < size_ips_t; i++)
            duples_subnet_connection[i] = NULL;

        // Populate
        for (int i = 0; i < duplets_size; i++) { // for each duple
            int pos = -1;
            pos = aliases_array[i].data1_pos;
            if (pos > -1)
                duples_subnet_connection[pos] = insert_list_binary_alias(duples_subnet_connection[pos], i);

            pos = -1;
            pos = aliases_array[i].data2_pos;
            if (pos > -1)
                duples_subnet_connection[pos] = insert_list_binary_alias(duples_subnet_connection[pos], i);
        }
        /////////
        // END //
        /////////


        /////////////////////////////////////////////
        // Print IPs and the duples they appear in //
        /////////////////////////////////////////////
        if (verbose) {
            printf("=========================================================================\n");
            printf("========================= DUPLES EACH IP ARE IN =========================\n");
            printf("=========================================================================\n\n");

            for (int i = 0; i < size_ips_t; i++) {
                printf("IP: %s\n", convert_int_to_ip(ips_t_array[i]));
                //print_list_binary_alias(duples_subnet_connection[i]);

                linkedlist_alias *ptr = duples_subnet_connection[i];
                //start from the beginning
                while(ptr != NULL) {
                    printf("\t%s, %s\n", convert_int_to_ip(aliases_array[ptr->data].data1), convert_int_to_ip(aliases_array[ptr->data].data2));
                    ptr = ptr->next;
                }

                printf("\n");
            }

            printf("====================\n");
            printf("Checking duple pairs\n");
            printf("====================\n\n");
        }
        /////////
        // END //
        /////////


        ////////////////////////
        // Load midar results //
        ////////////////////////
        if (midar_filename != NULL) { // if midar file is provided
            midar_infile = fopen(midar_filename, "r"); // open the file
            if (midar_infile == NULL) {
                printf("WARNING! Midar file pointer is null, skipping!\n");
                fprintf(midar_infile, "WARNING! Midar file pointer is null, skipping!\n");
            }
            else {
                // read MIDAR results
                char line[MAXSTRING]; // contains the line
                char *word = NULL;

                while (fgets(line, MAXSTRING, midar_infile) != NULL) { // for each entry
                    line[strlen(line) - 1] = '\0';
                    if (line[0] != '#' && line[0] != '\n') {
                        word = strtok(line, " \t\r\n\f\v"); // ip number
                        uint32_t IP1 = convert_ip_to_int(word);

                        word = strtok(NULL, " \t\r\n\f\v"); // ip number
                        uint32_t IP2 = convert_ip_to_int(word);

                        int potential_alias_node1_pos = find(ips_t_array, IP1, size_ips_t);
                        int potential_alias_node2_pos = find(ips_t_array, IP2, size_ips_t);

                        if (potential_alias_node1_pos != -1 && potential_alias_node2_pos != -1) {
                            if (alias_pos[potential_alias_node1_pos] != alias_pos[potential_alias_node2_pos]) {
                                // add to pos
                                linkedlist* temp = alias_tree[alias_pos[potential_alias_node2_pos]];
                                while (temp != NULL) {
                                    alias_tree[alias_pos[potential_alias_node1_pos]] = insert_list_alias(alias_tree[alias_pos[potential_alias_node1_pos]], temp->data2, temp->occurence);
                                    temp = temp->next;
                                }

                                deleteList(alias_tree[alias_pos[potential_alias_node2_pos]]);
                                alias_tree[alias_pos[potential_alias_node2_pos]] = NULL;

                                // find those with node2->mask and set to node1->mask in alias_pos
                                int tobereplaced = alias_pos[potential_alias_node2_pos];
                                int toreplace = alias_pos[potential_alias_node1_pos];
                                for (int i = 0; i < size_ips_t; i++)
                                    if (alias_pos[i] == tobereplaced)
                                        alias_pos[i] = toreplace;

                                // fix distance
                                for (int i = 0; i < size_vps_vp; i++) {
                                    int c_min = min(vps[i][potential_alias_node1_pos], vps[i][potential_alias_node2_pos]);
                                    vps[i][potential_alias_node1_pos] = c_min;
                                    vps[i][potential_alias_node2_pos] = c_min;
                                }

                                for (int i = 0; i < size_vps_ingress; i++) {
                                    int c_min = min(ingress[i][potential_alias_node1_pos], ingress[i][potential_alias_node2_pos]);
                                    ingress[i][potential_alias_node1_pos] = c_min;
                                    ingress[i][potential_alias_node2_pos] = c_min;
                                }

                                for (int i = 0; i < size_vps_egress; i++) {
                                    int c_min = min(egress[i][potential_alias_node1_pos], egress[i][potential_alias_node2_pos]);
                                    egress[i][potential_alias_node1_pos] = c_min;
                                    egress[i][potential_alias_node2_pos] = c_min;
                                }

                                // Merge conflict sets
                                int pos = 0;

                                int temp_size1 = size_tree_ip(conflict_set[conflict_set_positions[potential_alias_node1_pos]]);
                                uint32_t *temp_array1 = (uint32_t*) malloc(sizeof(uint32_t) * temp_size1);
                                pos = 0;
                                convert_tree_to_array_ip(conflict_set[conflict_set_positions[potential_alias_node1_pos]], temp_array1, &pos); // convert tree to array

                                int temp_size2 = size_tree_ip(conflict_set[conflict_set_positions[potential_alias_node2_pos]]);
                                uint32_t *temp_array2 = (uint32_t*) malloc(sizeof(uint32_t) * temp_size2);
                                pos = 0;
                                convert_tree_to_array_ip(conflict_set[conflict_set_positions[potential_alias_node2_pos]], temp_array2, &pos); // convert tree to array

                                // merge ips in the conflict set of ip1 with ip2
                                for (int i = 0; i < temp_size1; i++) {
                                    int pos = -1;
                                    pos = find(ips_t_array, temp_array1[i], size_ips_t);

                                    if (pos != -1)
                                        conflict_set[conflict_set_positions[pos]] = insert_tree_ip(conflict_set[conflict_set_positions[pos]], ips_t_array[potential_alias_node2_pos]);
                                }
                                // merge ips in the conflict set of ip2 with ip1
                                for (int i = 0; i < temp_size2; i++) {
                                    int pos = -1;
                                    pos = find(ips_t_array, temp_array2[i], size_ips_t);

                                    if (pos != -1)
                                        conflict_set[conflict_set_positions[pos]] = insert_tree_ip(conflict_set[conflict_set_positions[pos]], ips_t_array[potential_alias_node1_pos]);
                                }
                                // merge trees
                                for (int i = 0; i < temp_size2; i++)
                                    conflict_set[conflict_set_positions[potential_alias_node1_pos]] = insert_tree_ip(conflict_set[conflict_set_positions[potential_alias_node1_pos]], temp_array2[i]);
                                delete_tree_ip(conflict_set[conflict_set_positions[potential_alias_node2_pos]]);
                                conflict_set[conflict_set_positions[potential_alias_node2_pos]] = NULL;
                                for (int i = 0; i < size_ips_t; i++)
                                    if (conflict_set_positions[i] == conflict_set_positions[potential_alias_node2_pos])
                                        conflict_set_positions[i] = conflict_set_positions[potential_alias_node1_pos];
                                free(temp_array1);
                                free(temp_array2);

                                if (verbose) {
                                    printf("==Alias tree connections==\n");
                                    for (int o = 0; o < size_ips_t; o++) {
                                        printf("[%d] [%d]\n", o, alias_pos[o]);
                                    }
                                    printf("\n");

                                    for (int o = 0; o < size_ips_t; o++) {
                                        printf("[%d] -> ", o);
                                        print_list2(alias_tree[o]);
                                        printf("\n");
                                    }
                                    printf("\n");
                                }
                            }
                            else if (verbose)
                                printf("\t\t\t\t\tAlready aliases, so skipping.\n");
                        }
                        else {
                            printf("\t\t\t\t\tIPs in midar file not found, this shouldn't happen..\n");
                            exit(0);
                        }
                    }
                }

                fclose(midar_infile);
            }
        }
        /////////
        // END //
        /////////


        printf("Resolving aliases...\n\n");

        if (verbose) {
            printf("==Alias tree connections==\n");
            for (int o = 0; o < size_ips_t; o++) {
                printf("[%d] [%d]\n", o, alias_pos[o]);
            }
            printf("\n");

            for (int o = 0; o < size_ips_t; o++) {
                printf("[%d] -> ", o);
                print_list2(alias_tree[o]);
                printf("\n");
            }
            printf("\n");
        }

        // Resolve aliases
        for (int x = 0; x < no_of_steps; x++) { // determines the number of steps to go through
            int check = alias_checks[x];

            printf("Checking alias step %d...\n", check);
            bool changes_made = false;
            int loop = 0;

            do {
                changes_made = false;
                loop++;
                printf("Loop #%d...\n", loop);

                for (int i = 0; i < duplets_size; i++) { // for each duple
                    if (verbose)
                        printf("Duple: %s, %s\n", convert_int_to_ip(aliases_array[i].data1), convert_int_to_ip(aliases_array[i].data2));

                    /////////////////////////////////////////////////////////
                    // get the edges to be compared with the current duple //
                    /////////////////////////////////////////////////////////
                    binarytree_duple* inner_duples = NULL;
                    for (int edge = 0; edge < 2; edge++) { // for each side
                        // read IP1 and its pos
                        uint32_t IP1_left = 0;
                        uint32_t IP1_right = 0;
                        int IP1_left_pos = -1;
                        int IP1_right_pos = -1;

                        if (edge == 0) {
                            IP1_left = aliases_array[i].data1;
                            IP1_left_pos = aliases_array[i].data1_pos;
                            IP1_right = aliases_array[i].data2;
                            IP1_right_pos = aliases_array[i].data2_pos;
                        }
                        else {
                            IP1_left = aliases_array[i].data2;
                            IP1_left_pos = aliases_array[i].data2_pos;
                            IP1_right = aliases_array[i].data1;
                            IP1_right_pos = aliases_array[i].data1_pos;
                        }

                        // find the subnet no of the IP of the duple
                        int subnet_no = ips_subnet_id_array[IP1_left_pos];

                        // If the IP belongs to a subnet
                        if (subnet_no > 0) {
                            if (verbose)
                                printf("\tIPs in the same subnet (there are: %d):\n", (sizes[subnet_no-1]-1));

                            // get the IPs that the middle IP is in the same subnet with
                            uint32_t *ptr_temp = subnets_containing_ips_array[subnet_no-1];

                            for (int k = 0; k < sizes[subnet_no-1]; k++) { // for each ip in the subnet
                                if (!ips_equal(ptr_temp[k], IP1_left)) { // ignore if the IP is the same as the current IP being processed
                                    if (verbose)
                                        printf("\t\t%s\n", convert_int_to_ip(ptr_temp[k]));

                                    // Find the position of the IP that is in the same subnet as the current IP
                                    int pos2 = find(ips_t_array, ptr_temp[k], size_ips_t);

                                    // get the duples whose edges are in the same subnet as the middle IP
                                    linkedlist_alias *ptr = duples_subnet_connection[pos2];

                                    while (ptr != NULL) { // for each duple (the one to be compared to)
                                        if (verbose)
                                            printf("\t\t\t%s, %s\n", convert_int_to_ip(aliases_array[ptr->data].data1), convert_int_to_ip(aliases_array[ptr->data].data2));

                                        inner_duples = insert_tree_with_ip_duple(inner_duples, aliases_array[ptr->data].data1, aliases_array[ptr->data].data2, aliases_array[ptr->data].data1_pos, aliases_array[ptr->data].data2_pos, aliases_array[ptr->data].completeness, aliases_array[ptr->data].reference); // insert duple
                                        ptr = ptr->next; // go to the next duple
                                    }

                                    if (verbose)
                                        printf("\n");
                                }
                            }
                        }
                        else if (verbose)
                            printf("\tIP does not belong to subnet, skipping!\n\n");
                    }
                    /////////
                    // END //
                    /////////



                    //////////////////////////////////
                    // Convert duples tree to array //
                    //////////////////////////////////
                    int inner_duplets_size = size_tree_duple(inner_duples); // number of aliases
                    duple* inner_aliases_array = (duple*) malloc(sizeof(duple) * inner_duplets_size); // holds the ips and their masks (as array)
                    pos = 0; // position to add to the array
                    convert_duple_tree_to_array(inner_duples, inner_aliases_array, &pos); // convert tree to array
                    delete_tree_duple(inner_duples); // delete the tree
                    /////////
                    // END //
                    /////////


                    ////////////////////////////////////////////////////////////////
                    // Sort the duples by their reference points and completeness //
                    ////////////////////////////////////////////////////////////////
                    duple_completeness_mergesort(inner_aliases_array, 0, inner_duplets_size-1);
                    duple_reference_mergesort(inner_aliases_array, 0, inner_duplets_size-1);
                    /////////
                    // END //
                    /////////

                    if (verbose) {
                        printf("\t\tSorted duples\n");
                        for (int i = 0; i < inner_duplets_size; i++)
                            printf("\t\t%s, %s\n", convert_int_to_ip(inner_aliases_array[i].data1), convert_int_to_ip(inner_aliases_array[i].data2));
                    }

                    if (verbose)
                        printf("RESOLVING NOW.........\n\n");

                    /////////////
                    // RESOLVE //
                    /////////////
                    for (int edge = 0; edge < 2; edge++) { // for each side
                        // read IP1 and its pos
                        uint32_t IP1_left = 0;
                        uint32_t IP1_right = 0;
                        int IP1_left_pos = -1;
                        int IP1_right_pos = -1;

                        if (edge == 0) {
                            if (verbose)
                                printf("IP LEFT CHOSEN!!!!!!!\n");
                            IP1_left = aliases_array[i].data1;
                            IP1_left_pos = aliases_array[i].data1_pos;
                            IP1_right = aliases_array[i].data2;
                            IP1_right_pos = aliases_array[i].data2_pos;
                        }
                        else {
                            if (verbose)
                                printf("IP RIGHT CHOSEN!!!!!!!\n");
                            IP1_left = aliases_array[i].data2;
                            IP1_left_pos = aliases_array[i].data2_pos;
                            IP1_right = aliases_array[i].data1;
                            IP1_right_pos = aliases_array[i].data1_pos;
                        }

                        if (!ip_is_zero(IP1_left)) { // if the IP is not 0
                            for (int in_duple = 0; in_duple < inner_duplets_size; in_duple++) { // for each duple (the one to be compared to)
                                uint32_t IP2_left = -1;
                                uint32_t IP2_right = -1;
                                int IP2_left_pos = -1;
                                int IP2_right_pos = -1;
                                int potential_alias_node1_pos = -1;
                                int potential_alias_node2_pos = -1;

                                // if the left IP of the first triplet and the first IP of the second triplet are in the same subnet
                                if (ips_subnet_id_array[IP1_left_pos] == ips_subnet_id_array[inner_aliases_array[in_duple].data1_pos] && ips_subnet_id_array[IP1_left_pos] > 0) {
                                    IP2_left = inner_aliases_array[in_duple].data1;
                                    IP2_left_pos = inner_aliases_array[in_duple].data1_pos;
                                    IP2_right = inner_aliases_array[in_duple].data2;
                                    IP2_right_pos = inner_aliases_array[in_duple].data2_pos;
                                }
                                else if (ips_subnet_id_array[IP1_left_pos] == ips_subnet_id_array[inner_aliases_array[in_duple].data2_pos] && ips_subnet_id_array[IP1_left_pos] > 0) {
                                    IP2_left = inner_aliases_array[in_duple].data2;
                                    IP2_left_pos = inner_aliases_array[in_duple].data2_pos;
                                    IP2_right = inner_aliases_array[in_duple].data1;
                                    IP2_right_pos = inner_aliases_array[in_duple].data1_pos;
                                }

                                if (IP2_left != -1 && IP2_right != -1 && IP2_left_pos != -1 && IP2_right_pos != -1) {
                                    if (verbose) {
                                        printf("\t\t\t%s, %s\n", convert_int_to_ip(IP1_left), convert_int_to_ip(IP1_right));
                                        printf("\t\t\t%s, %s\n", convert_int_to_ip(IP2_left), convert_int_to_ip(IP2_right));
                                    }

                                    if (!ip_is_zero(IP2_left)) {
                                        if (IP1_right != IP2_right) {
                                            // if none of the cross IPs are already aliases
                                            if (alias_pos[IP1_left_pos] != alias_pos[IP2_right_pos] && alias_pos[IP1_right_pos] != alias_pos[IP2_left_pos]) {
                                                bool to_be_processed_1and4 = false;
                                                bool to_be_processed_2and3 = false;

                                                if (check == 30 || check == 31 || check == 32) { // point-to-point
                                                    // if subnet mask >= 30
                                                    if (subnets_array[ips_subnet_id_array[IP1_left_pos]].mask >= 30) {
                                                        to_be_processed_1and4 = true;
                                                        to_be_processed_2and3 = true;

                                                        if (verbose)
                                                            printf("\t\t\t\tWill consider this pair because the left side fall under /30 or /31 subnet.\n");
                                                    }
                                                }
                                                else if (check == 1 || check == 2 || check == 6 || check == 9 || check == 10) { // s-s
                                                    // check if the other edge is in same subnet
                                                    if (ips_subnet_id_array[IP1_right_pos] == ips_subnet_id_array[IP2_right_pos] && ips_subnet_id_array[IP1_right_pos] > 0) {
                                                        to_be_processed_1and4 = true;
                                                        to_be_processed_2and3 = true;

                                                        if (verbose)
                                                            printf("\t\t\t\tWill consider this pair because the right IPs fall under the same subnet.\n");
                                                    }
                                                }
                                                else if (check == 4 || check == 5 || check == 7 || check == 11 || check == 12) { // s-a
                                                    linkedlist *temp = neighbors[IP1_right_pos];
                                                    while (temp!=NULL) {
                                                        if (temp->data2 != IP1_left && (temp->data2 == IP2_right || alias_pos[temp->occurence] == alias_pos[IP2_right_pos])) {
                                                            to_be_processed_2and3 = true;

                                                            if (verbose)
                                                                printf("\t\t\t\tWill consider this pair because IP1_right and IP2_left are aliases.\n");

                                                            break;
                                                        }
                                                        temp = temp->next;
                                                    }

                                                    temp = neighbors[IP2_right_pos];
                                                    while (temp!=NULL) {
                                                        if (temp->data2 != IP2_left && (temp->data2 == IP1_right || alias_pos[temp->occurence] == alias_pos[IP1_right_pos])) {
                                                            to_be_processed_1and4 = true;

                                                            if (verbose)
                                                                printf("\t\t\t\tWill consider this pair because IP1_left and IP2_right are aliases.\n");

                                                            break;
                                                        }
                                                        temp = temp->next;
                                                    }
                                                }
                                                else {
                                                    printf("Error! Wrong check number provided!\n");
                                                    exit(0);
                                                }

                                                if (to_be_processed_1and4 || to_be_processed_2and3) {
                                                    ///////////////////////////////////////////////
                                                    // Calculate distances for potential aliases //
                                                    ///////////////////////////////////////////////

                                                    // distance
                                                    int no_of_dist_match_for_1and4 = 0;
                                                    int no_of_dist_match_for_2and3 = 0;

                                                    // conflict
                                                    bool no_of_conf_match_for_1and4 = false;
                                                    bool no_of_conf_match_for_2and3 = false;

                                                    if (to_be_processed_1and4) {
                                                        // for 1 and 4
                                                        if (no_of_dist_match_for_1and4 != -1) {
                                                            for (int i = 0; i < size_vps_vp; i++) {
                                                                if (vps[i][IP1_left_pos] != 255 && vps[i][IP2_right_pos] != 255) {
                                                                    if (abs(vps[i][IP1_left_pos] - vps[i][IP2_right_pos]) >= 1) {
                                                                        no_of_dist_match_for_1and4 = -1;
                                                                        break;
                                                                    }
                                                                    else
                                                                        no_of_dist_match_for_1and4++;
                                                                }
                                                            }
                                                        }

                                                        if (no_of_dist_match_for_1and4 != -1) {
                                                            for (int i = 0; i < size_vps_ingress; i++) {
                                                                if (ingress[i][IP1_left_pos] != 255 && ingress[i][IP2_right_pos] != 255) {
                                                                    if (abs(ingress[i][IP1_left_pos] - ingress[i][IP2_right_pos]) >= 1) {
                                                                        no_of_dist_match_for_1and4 = -1;
                                                                        break;
                                                                    }
                                                                    else
                                                                        no_of_dist_match_for_1and4++;
                                                                }
                                                            }
                                                        }

                                                        if (no_of_dist_match_for_1and4 != -1) {
                                                            for (int i = 0; i < size_vps_egress; i++) {
                                                                if (egress[i][IP1_left_pos] != 255 && egress[i][IP2_right_pos] != 255) {
                                                                    if (abs(egress[i][IP1_left_pos] - egress[i][IP2_right_pos]) >= 1) {
                                                                        no_of_dist_match_for_1and4 = -1;
                                                                        break;
                                                                    }
                                                                    else
                                                                        no_of_dist_match_for_1and4++;
                                                                }
                                                            }
                                                        }

                                                        if (verbose)
                                                            printf("\t\t\tdistance match for first potential IP1 and IP2: %d\n", no_of_dist_match_for_1and4);

                                                        if (!ips_conflict_exists(conflict_set, ips_t_array[IP1_left_pos], ips_t_array[IP2_right_pos], IP1_left_pos, IP2_right_pos, conflict_set_positions))
                                                            no_of_conf_match_for_1and4 = true;

                                                        if (verbose) {
                                                            if (no_of_conf_match_for_1and4)
                                                                printf("\t\t\tconflict match for first potential IP1 and IP2 is true\n");
                                                            else
                                                                printf("\t\t\tconflict match for first potential IP1 and IP2 is false\n");
                                                        }
                                                    }

                                                    if (to_be_processed_2and3) {
                                                        // for 2 and 3
                                                        if (no_of_dist_match_for_2and3 != -1) {
                                                            for (int i = 0; i < size_vps_vp; i++) {
                                                                if (vps[i][IP1_right_pos] != 255 && vps[i][IP2_left_pos] != 255) {
                                                                    if (abs(vps[i][IP1_right_pos] - vps[i][IP2_left_pos]) >= 1) {
                                                                        no_of_dist_match_for_2and3 = -1;
                                                                        break;
                                                                    }
                                                                    else
                                                                        no_of_dist_match_for_2and3++;
                                                                }
                                                            }
                                                        }

                                                        if (no_of_dist_match_for_2and3 != -1) {
                                                            for (int i = 0; i < size_vps_ingress; i++) {
                                                                if (ingress[i][IP1_right_pos] != 255 && ingress[i][IP2_left_pos] != 255) {
                                                                    if (abs(ingress[i][IP1_right_pos] - ingress[i][IP2_left_pos]) >= 1) {
                                                                        no_of_dist_match_for_2and3 = -1;
                                                                        break;
                                                                    }
                                                                    else
                                                                        no_of_dist_match_for_2and3++;
                                                                }
                                                            }
                                                        }

                                                        if (no_of_dist_match_for_2and3 != -1) {
                                                            for (int i = 0; i < size_vps_egress; i++) {
                                                                if (egress[i][IP1_right_pos] != 255 && egress[i][IP2_left_pos] != 255) {
                                                                    if (abs(egress[i][IP1_right_pos] - egress[i][IP2_left_pos]) >= 1) {
                                                                        no_of_dist_match_for_2and3 = -1;
                                                                        break;
                                                                    }
                                                                    else
                                                                        no_of_dist_match_for_2and3++;
                                                                }
                                                            }
                                                        }

                                                        if (verbose)
                                                            printf("\t\t\tdistance match for first potential IP1 and IP2: %d\n", no_of_dist_match_for_2and3);

                                                        if (!ips_conflict_exists(conflict_set, ips_t_array[IP1_right_pos], ips_t_array[IP2_left_pos], IP1_right_pos, IP2_left_pos, conflict_set_positions))
                                                            no_of_conf_match_for_2and3 = true;

                                                        if (verbose) {
                                                            if (no_of_conf_match_for_2and3)
                                                                printf("\t\t\tconflict match for first potential IP1 and IP2 is true\n");
                                                            else
                                                                printf("\t\t\tconflict match for first potential IP1 and IP2 is false\n");
                                                        }
                                                    }


                                                    /////////////////////////////////////////////////////////////////////////////
                                                    // Decide which pair is an alias or if they should go to unclassified list //
                                                    /////////////////////////////////////////////////////////////////////////////
                                                    if (check == 1 || check == 4) {
                                                        // Step 1
                                                        if (to_be_processed_1and4 && to_be_processed_2and3) {
                                                            if (no_of_dist_match_for_1and4 > 0 && no_of_dist_match_for_2and3 < 0) {
                                                                potential_alias_node1_pos = IP1_left_pos;
                                                                potential_alias_node2_pos = IP2_right_pos;
                                                            }
                                                            else if (no_of_dist_match_for_2and3 > 0 && no_of_dist_match_for_1and4 < 0) {
                                                                potential_alias_node1_pos = IP1_right_pos;
                                                                potential_alias_node2_pos = IP2_left_pos;
                                                            }
                                                        }
                                                    }
                                                    else if (check == 2 || check == 5) {
                                                        // Step 1
                                                        if (to_be_processed_1and4 && to_be_processed_2and3) {
                                                            if (no_of_dist_match_for_1and4 > 0 && no_of_dist_match_for_2and3 <= 0) {
                                                                potential_alias_node1_pos = IP1_left_pos;
                                                                potential_alias_node2_pos = IP2_right_pos;
                                                            }
                                                            else if (no_of_dist_match_for_2and3 > 0 && no_of_dist_match_for_1and4 <= 0) {
                                                                potential_alias_node1_pos = IP1_right_pos;
                                                                potential_alias_node2_pos = IP2_left_pos;
                                                            }
                                                        }
                                                    }
                                                    else if (check == 6 || check == 7) {
                                                        // Step 1
                                                        if (to_be_processed_1and4 && to_be_processed_2and3) {
                                                            if (no_of_conf_match_for_1and4 && !no_of_conf_match_for_2and3) {
                                                                potential_alias_node1_pos = IP1_left_pos;
                                                                potential_alias_node2_pos = IP2_right_pos;
                                                            }
                                                            else if (no_of_conf_match_for_2and3 && !no_of_conf_match_for_1and4) {
                                                                potential_alias_node1_pos = IP1_right_pos;
                                                                potential_alias_node2_pos = IP2_left_pos;
                                                            }
                                                        }
                                                    }
                                                    else if (check == 9 || check == 11) {
                                                        // Step 1
                                                        if (to_be_processed_1and4 && to_be_processed_2and3) {
                                                            if (no_of_dist_match_for_1and4 > 0 && no_of_dist_match_for_2and3 < 0 && no_of_conf_match_for_1and4 && !no_of_conf_match_for_2and3) {
                                                                potential_alias_node1_pos = IP1_left_pos;
                                                                potential_alias_node2_pos = IP2_right_pos;
                                                            }
                                                            else if (no_of_dist_match_for_2and3 > 0 && no_of_dist_match_for_1and4 < 0 && no_of_conf_match_for_2and3 && !no_of_conf_match_for_1and4) {
                                                                potential_alias_node1_pos = IP1_right_pos;
                                                                potential_alias_node2_pos = IP2_left_pos;
                                                            }
                                                        }
                                                    }
                                                    else if (check == 10 || check == 12) {
                                                        // Step 1
                                                        if (to_be_processed_1and4 && to_be_processed_2and3) {
                                                            if (no_of_dist_match_for_1and4 > 0 && no_of_dist_match_for_2and3 <= 0 && no_of_conf_match_for_1and4 && !no_of_conf_match_for_2and3) {
                                                                potential_alias_node1_pos = IP1_left_pos;
                                                                potential_alias_node2_pos = IP2_right_pos;
                                                            }
                                                            else if (no_of_dist_match_for_2and3 > 0 && no_of_dist_match_for_1and4 <= 0 && no_of_conf_match_for_2and3 && !no_of_conf_match_for_1and4) {
                                                                potential_alias_node1_pos = IP1_right_pos;
                                                                potential_alias_node2_pos = IP2_left_pos;
                                                            }
                                                        }
                                                    }
                                                    else if (check == 30) {
                                                        // Step 1
                                                        if (to_be_processed_1and4 && to_be_processed_2and3) {
                                                            if (no_of_dist_match_for_1and4 > 0 && (alias_pos[IP1_left_pos] != alias_pos[IP2_right_pos])) {
                                                                potential_alias_node1_pos = IP1_left_pos;
                                                                potential_alias_node2_pos = IP2_right_pos;
                                                            }
                                                            else if (no_of_dist_match_for_2and3 > 0 && (alias_pos[IP1_right_pos] != alias_pos[IP2_left_pos])) {
                                                                potential_alias_node1_pos = IP1_right_pos;
                                                                potential_alias_node2_pos = IP2_left_pos;
                                                            }
                                                        }
                                                    }
                                                    else if (check == 31) {
                                                        // Step 1
                                                        if (to_be_processed_1and4 && to_be_processed_2and3) {
                                                            if (no_of_conf_match_for_1and4 && (alias_pos[IP1_left_pos] != alias_pos[IP2_right_pos])) {
                                                                potential_alias_node1_pos = IP1_left_pos;
                                                                potential_alias_node2_pos = IP2_right_pos;
                                                            }
                                                            else if (no_of_conf_match_for_2and3 && (alias_pos[IP1_right_pos] != alias_pos[IP2_left_pos])) {
                                                                potential_alias_node1_pos = IP1_right_pos;
                                                                potential_alias_node2_pos = IP2_left_pos;
                                                            }
                                                        }
                                                    }
                                                    else if (check == 32) {
                                                        // Step 1
                                                        if (to_be_processed_1and4 && to_be_processed_2and3) {
                                                            if (no_of_dist_match_for_1and4 > 0 && no_of_conf_match_for_1and4 && (alias_pos[IP1_left_pos] != alias_pos[IP2_right_pos])) {
                                                                potential_alias_node1_pos = IP1_left_pos;
                                                                potential_alias_node2_pos = IP2_right_pos;
                                                            }
                                                            else if (no_of_dist_match_for_2and3 > 0 && no_of_conf_match_for_2and3 && (alias_pos[IP1_right_pos] != alias_pos[IP2_left_pos])) {
                                                                potential_alias_node1_pos = IP1_right_pos;
                                                                potential_alias_node2_pos = IP2_left_pos;
                                                            }
                                                        }
                                                    }
                                                    /////////
                                                    // END //
                                                    /////////


                                                    // if any of them are true, then add the middle IPs to alias list
                                                    if (potential_alias_node1_pos != -1 && potential_alias_node2_pos != -1) {
                                                        if (verbose)
                                                            printf("\n\t\t\t\t\t([%d][%d]%s, [%d][%d]%s) are aliases\n", potential_alias_node1_pos, alias_pos[potential_alias_node1_pos], convert_int_to_ip(ips_t_array[potential_alias_node1_pos]), potential_alias_node2_pos, alias_pos[potential_alias_node2_pos], convert_int_to_ip(ips_t_array[potential_alias_node2_pos]));

                                                        // if both exists and are in different entries, then merge
                                                        if (alias_pos[potential_alias_node1_pos] != alias_pos[potential_alias_node2_pos]) {
                                                            changes_made = true;

                                                            // add to pos
                                                            linkedlist* temp = alias_tree[alias_pos[potential_alias_node2_pos]];
                                                            while (temp != NULL) {
                                                                alias_tree[alias_pos[potential_alias_node1_pos]] = insert_list_alias(alias_tree[alias_pos[potential_alias_node1_pos]], temp->data2, temp->occurence);
                                                                temp = temp->next;
                                                            }

                                                            deleteList(alias_tree[alias_pos[potential_alias_node2_pos]]);
                                                            alias_tree[alias_pos[potential_alias_node2_pos]] = NULL;

                                                            // find those with node2->mask and set to node1->mask in alias_pos
                                                            int tobereplaced = alias_pos[potential_alias_node2_pos];
                                                            int toreplace = alias_pos[potential_alias_node1_pos];
                                                            for (int i = 0; i < size_ips_t; i++)
                                                                if (alias_pos[i] == tobereplaced)
                                                                    alias_pos[i] = toreplace;

                                                            // fix distance
                                                            for (int i = 0; i < size_vps_vp; i++) {
                                                                int c_min = min(vps[i][potential_alias_node1_pos], vps[i][potential_alias_node2_pos]);
                                                                vps[i][potential_alias_node1_pos] = c_min;
                                                                vps[i][potential_alias_node2_pos] = c_min;
                                                            }

                                                            for (int i = 0; i < size_vps_ingress; i++) {
                                                                int c_min = min(ingress[i][potential_alias_node1_pos], ingress[i][potential_alias_node2_pos]);
                                                                ingress[i][potential_alias_node1_pos] = c_min;
                                                                ingress[i][potential_alias_node2_pos] = c_min;
                                                            }

                                                            for (int i = 0; i < size_vps_egress; i++) {
                                                                int c_min = min(egress[i][potential_alias_node1_pos], egress[i][potential_alias_node2_pos]);
                                                                egress[i][potential_alias_node1_pos] = c_min;
                                                                egress[i][potential_alias_node2_pos] = c_min;
                                                            }

                                                            // Merge conflict sets
                                                            int pos = 0;

                                                            int temp_size1 = size_tree_ip(conflict_set[conflict_set_positions[potential_alias_node1_pos]]);
                                                            uint32_t *temp_array1 = (uint32_t*) malloc(sizeof(uint32_t) * temp_size1);
                                                            pos = 0;
                                                            convert_tree_to_array_ip(conflict_set[conflict_set_positions[potential_alias_node1_pos]], temp_array1, &pos); // convert tree to array

                                                            int temp_size2 = size_tree_ip(conflict_set[conflict_set_positions[potential_alias_node2_pos]]);
                                                            uint32_t *temp_array2 = (uint32_t*) malloc(sizeof(uint32_t) * temp_size2);
                                                            pos = 0;
                                                            convert_tree_to_array_ip(conflict_set[conflict_set_positions[potential_alias_node2_pos]], temp_array2, &pos); // convert tree to array

                                                            // merge ips in the conflict set of ip1 with ip2
                                                            for (int i = 0; i < temp_size1; i++) {
                                                                int pos = -1;
                                                                pos = find(ips_t_array, temp_array1[i], size_ips_t);

                                                                if (pos != -1)
                                                                    conflict_set[conflict_set_positions[pos]] = insert_tree_ip(conflict_set[conflict_set_positions[pos]], ips_t_array[potential_alias_node2_pos]);
                                                            }
                                                            // merge ips in the conflict set of ip2 with ip1
                                                            for (int i = 0; i < temp_size2; i++) {
                                                                int pos = -1;
                                                                pos = find(ips_t_array, temp_array2[i], size_ips_t);

                                                                if (pos != -1)
                                                                    conflict_set[conflict_set_positions[pos]] = insert_tree_ip(conflict_set[conflict_set_positions[pos]], ips_t_array[potential_alias_node1_pos]);
                                                            }
                                                            // merge trees
                                                            for (int i = 0; i < temp_size2; i++)
                                                                conflict_set[conflict_set_positions[potential_alias_node1_pos]] = insert_tree_ip(conflict_set[conflict_set_positions[potential_alias_node1_pos]], temp_array2[i]);
                                                            delete_tree_ip(conflict_set[conflict_set_positions[potential_alias_node2_pos]]);
                                                            conflict_set[conflict_set_positions[potential_alias_node2_pos]] = NULL;
                                                            for (int i = 0; i < size_ips_t; i++)
                                                                if (conflict_set_positions[i] == conflict_set_positions[potential_alias_node2_pos])
                                                                    conflict_set_positions[i] = conflict_set_positions[potential_alias_node1_pos];
                                                            free(temp_array1);
                                                            free(temp_array2);

                                                            /*
                                                            printf("==Alias tree connections==\n");
                                                            for (int o = 0; o < size_ips_t; o++) {
                                                                printf("[%d] [%d]\n", o, alias_pos[o]);
                                                            }
                                                            printf("\n");

                                                            for (int o = 0; o < size_ips_t; o++) {
                                                                printf("[%d] -> ", o);
                                                                print_list2(alias_tree[o]);
                                                                printf("\n");
                                                            }
                                                            printf("\n");
                                                            */
                                                        }
                                                        else if (verbose)
                                                            printf("\t\t\t\t\tAlready aliases, so skipping.\n");
                                                    }
                                                    else if (verbose)
                                                        printf("\t\t\t\tSkipping.\n");
                                                }
                                                else if (verbose)
                                                    printf("\t\t\t\tSkipping since neither 1,4 or 2,3 are to be considered\n");
                                            }
                                            else if (verbose)
                                                printf("\t\t\t\tOne of the cross IPs have already been added as aliases!\n");
                                        }
                                        else if (verbose)
                                            printf("\t\t\t\tSkipping since right IPs are the same!\n");
                                    }
                                }
                            }

                            if (verbose)
                                printf("\n");
                        }
                        else if (verbose)
                            printf("ERROR IP is 0\n\n");
                    }
                    free(inner_aliases_array);
                }
            } while (changes_made);
            ///////////////////////////
            // Write aliases to file //
            ///////////////////////////
            if (write_every_step) {
                // open file stream
                char c[num_places(x+1) + 5];
                sprintf(c, "_%d.txt", x+1);
                aliases_outfile = fopen(return_filename_string(alias_output_filename, c), "w"); // open the file

                // write
                for (int i = 0; i < size_ips_t; i++) {
                    if (alias_tree[i] != NULL) {
                        if (!only_pair_aliases || ((only_pair_aliases) && alias_tree[i]->next != NULL)) {
                            print_list2_file(alias_tree[i], aliases_outfile);
                            fprintf(aliases_outfile, "\n");
                        }
                    }
                }

                // close
                fclose(aliases_outfile);
            }
        }

        ///////////////////////////
        // Write aliases to file //
        ///////////////////////////
        if (!write_every_step) {
            // open file stream
            aliases_outfile = fopen(return_filename_string(alias_output_filename, ".txt"), "w"); // open the file

            // write
            for (int i = 0; i < size_ips_t; i++) {
                if (alias_tree[i] != NULL) {
                    if (!only_pair_aliases || ((only_pair_aliases) && alias_tree[i]->next != NULL)) {
                        print_list2_file(alias_tree[i], aliases_outfile);
                        fprintf(aliases_outfile, "\n");
                    }
                }
            }

            // close
            fclose(aliases_outfile);
        }
        /////////
        // END //
        /////////


        free(aliases_array);
        printf("Aliases generated!\n");
        fprintf(logs_outfile, "Aliases generated!\n");
    }
    else if (verbose)
        printf("There are either no subnets, or it is in 'only-subnet' mode so no alias resolution applied!\n");

    fclose(logs_outfile);

    return 0;
}
