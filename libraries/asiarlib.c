#include "asiarlib.h"

binarytree_ip* get_ground_truth_ips(char const* ground_truth_ips_filename) {
    // Read file
    char line[MAXSTRING]; // contains the line
    binarytree_ip* ips = NULL;
    FILE* infile = fopen(ground_truth_ips_filename, "r");

    if (infile == NULL) {
        printf("WARNING! Ground truth file pointer is null, skipping!\n");
        exit(0);
    }

    while (fgets(line, MAXSTRING, infile) != NULL) { // for each subnet
        if (line[0] != '\n') { // if line not empty
            if (!isValidIp4(line))
                ips = insert_tree_ip(ips, convert_ip_to_int(line));
        }
    }

    fclose(infile); // close the stream
    return ips;
}

void print_separators(int separators_size, separators_struct* separators, uint32_t *ips_t_array, uint32_t *mask_one, uint32_t *mask_zero, int size_ips_t) {
    for (int i = 0; i < separators_size; i++) { // for each pair
        //int mask = get_mask_matches(i, i+1, mask_one, ips_t_array);
        int mask = check_special_ips(i, i+1, false, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, false);

        if (separators[i].state) {
            printf("%d: true (%s)", i, convert_int_to_ip(ips_t_array[i]));
            printf("\t\t\tmask: %d", mask);
            printf("\n");
        }
        else {
             printf("%d: \tfalse (%s)", i, convert_int_to_ip(ips_t_array[i]));
             printf("\t\tmask: %d", mask);
             printf("\n");
         }
    }
}

void determine_separators(int mode, int separators_size, uint32_t *ips_t_array, separators_struct* separators, uint32_t *mask_one, uint32_t *mask_zero, int size_vps_vp, short **vps, int size_vps_ingress, short **ingress, int size_vps_egress, short **egress, int subnet_pair_error_threshold, binarytree_ip **conflict_set_subnet, int* conflict_set_subnet_positions, int size_ips_t) {
    if (mode == 1) {
        for (int i = 0; i < separators_size; i++) { // for each pair
            if (ip_is_zero(ips_t_array[i]))
                separators[i].state = false;
            else {
                separators[i].state = true; // initialize it to true

                // do not bother with special ips here
                //int mask = get_mask_matches(i, i+1, mask_one, ips_t_array);
                int mask = check_special_ips(i, i+1, false, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, false);

                if (mask < 31) {
                    int no_of_non_matches = 0;
                    int total = 0;

                    // VP
                    for (int j = 0; j < size_vps_vp; j++) {
                        if (vps[j][i] != 255 || vps[j][i+1] != 255) // if at least one of the IPs were seen
                            total++;

                        if (vps[j][i] != 255 && vps[j][i+1] != 255) // if both of the IPs were seen
                            if (abs(vps[j][i] - vps[j][i+1]) > 1)
                                no_of_non_matches++;
                    }

                    // Ingress
                    for (int j = 0; j < size_vps_ingress; j++) {
                        if (ingress[j][i] != 255 || ingress[j][i+1] != 255) // if at least one of the IPs were seen
                            total++;

                        if (ingress[j][i] != 255 && ingress[j][i+1] != 255) // if both of the IPs were seen
                            if (abs(ingress[j][i] - ingress[j][i+1]) > 1)
                                no_of_non_matches++;
                    }

                    // Egress
                    for (int j = 0; j < size_vps_egress; j++) {
                        if (egress[j][i] != 255 || egress[j][i+1] != 255) // if at least one of the IPs were seen
                            total++;

                        if (egress[j][i] != 255 && egress[j][i+1] != 255) // if both of the IPs were seen
                            if (abs(egress[j][i] - egress[j][i+1]) > 1)
                                no_of_non_matches++;
                    }

                    if (((double)(no_of_non_matches) / (double)(total)) > subnet_pair_error_threshold)
                        separators[i].state = false;
                }
            }
        }
    }
    else if (mode == 2) {
        for (int i = 0; i < separators_size; i++) { // for each pair
            if (ip_is_zero(ips_t_array[i]))
                separators[i].state = false;
            else {
                separators[i].state = true; // initialize it to true

                // do not bother with special ips here
                //int mask = get_mask_matches(i, i+1, mask_one, ips_t_array);
                int mask = check_special_ips(i, i+1, false, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, false);

                if (mask < 31)
                    if (ips_conflict_exists(conflict_set_subnet, ips_t_array[i], ips_t_array[i+1], i, i+1, conflict_set_subnet_positions))
                        separators[i].state = false;
            }
        }
    }
    else if (mode == 3) {
        for (int i = 0; i < separators_size; i++) { // for each pair
            if (ip_is_zero(ips_t_array[i]))
                separators[i].state = false;
            else {
                separators[i].state = true; // initialize it to true

                // do not bother with special ips here
                //int mask = get_mask_matches(i, i+1, mask_one, ips_t_array);
                int mask = check_special_ips(i, i+1, false, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, false);

                if (mask < 31) {
                    int no_of_non_matches = 0;
                    int total = 0;

                    // VP
                    for (int j = 0; j < size_vps_vp; j++) {
                        if (vps[j][i] != 255 || vps[j][i+1] != 255) // if at least one of the IPs were seen
                            total++;

                        if (vps[j][i] != 255 && vps[j][i+1] != 255) // if both of the IPs were seen
                            if (abs(vps[j][i] - vps[j][i+1]) > 1)
                                no_of_non_matches++;
                    }

                    // Ingress
                    for (int j = 0; j < size_vps_ingress; j++) {
                        if (ingress[j][i] != 255 || ingress[j][i+1] != 255) // if at least one of the IPs were seen
                            total++;

                        if (ingress[j][i] != 255 && ingress[j][i+1] != 255) // if both of the IPs were seen
                            if (abs(ingress[j][i] - ingress[j][i+1]) > 1)
                                no_of_non_matches++;
                    }

                    // Egress
                    for (int j = 0; j < size_vps_egress; j++) {
                        if (egress[j][i] != 255 || egress[j][i+1] != 255) // if at least one of the IPs were seen
                            total++;

                        if (egress[j][i] != 255 && egress[j][i+1] != 255) // if both of the IPs were seen
                            if (abs(egress[j][i] - egress[j][i+1]) > 1)
                                no_of_non_matches++;
                    }

                    if ((((double)(no_of_non_matches) / (double)(total)) > subnet_pair_error_threshold) ||
                        ips_conflict_exists(conflict_set_subnet, ips_t_array[i], ips_t_array[i+1], i, i+1, conflict_set_subnet_positions)) {
                        //printf("SPLIT 7\n");
                        separators[i].state = false;
                    }
                }
            }
        }
    }
}

void help() {
    printf("ASIAR - Analytical Subnet Inference and Alias Resolution: 1.0\n");
    printf("Department of Computer Science & Engineering, University of Nevada, Reno\n");
    printf("Nevada/USA 2017\n");
    printf("\n");
    printf("AUTHOR\n");
    printf("\tAhmet Aksoy, Mehmet Hadi Gunes\n");
    printf("\n");
    printf("SYNOPSIS\n");
    printf("\tcode [OPTIONS]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("\t-h\t--help\t\t\t\tShows help page\n");
    printf("\t-i\t--input\t\t\t\tTrace input file\n");
    printf("\t-op\t--only-pairs\t\t\t\tPrint aliases with single IPs as well. Default: only aliases with >= 2 IPs are printed.\n");
    printf("\t-os\t--only-subnet-resolution\t\t\t\tPerform subnet resolution alone.\n");
    printf("\t-so\t--subnet-output\t\t\t\tSubnet output file\n");
    printf("\t-sod\t--subnet-output-detailed\t\t\t\tDetailed subnet output file (containing the IPs that fall in the subnets).\n");
    printf("\t-soe\t--subnet-output-edges\t\t\t\tPrint the edge IPs for each subnet.\n");
    printf("\t-lo\t--log-output\t\t\t\tLog output file\n");
    printf("\t-ap\t--alias-parameters\t\t\t\tAlias steps to go over when resolving aliases. e.g. 1,2,5,etc.\n");
    printf("\t-ns\t--no-of-steps\t\t\t\tNo. of steps to perform for alias resolution\n");
    printf("\t-ao\t--alias-output\t\t\t\tAlias output file\n");
    printf("\t-mi\t--midar-input\t\t\t\tMidar input file\n");
    printf("\t-ai\t--announcement-input\t\tSpecify the direct path of the announcement file (if not provided, testing mode is set to true)\n");
    printf("\t-as\t--as\t\t\tAS number (needed if not in test-mode)\n");
    printf("\t-ct\t--completeness-threshold\tCompleteness threshold for subnet inference\n");
    printf("\t-sp\t--subnet-pair-threshold\t...\n");
    printf("\t-sg\t--subnet-group-threshold\t...\n");
}

// find the position of the end of the subnet block
int find_end(separators_struct* separators, int size, int start) {
    int i = start + 1;

    // skip trues
    while ((i < size) && separators[i].state)
        i++;

    if (i >= size)
        i = size - 1;

    if (separators[i].state)
        return (i+1);
    else
        return i;
}

// checks if two IPs are in the same subnet
bool is_within_same_subnet(uint32_t data1, uint32_t data2, subnet* subnets_array, int subnets_size, uint32_t *mask_one) {
    if (ip_is_zero(data1) || ip_is_zero(data2))
        return false;

    for (int i = 0; i < subnets_size; i++)
        if (contains(subnets_array[i].data, subnets_array[i].mask, data1, mask_one) && contains(subnets_array[i].data, subnets_array[i].mask, data2, mask_one))
            return true;

    return false;
}

// check if 2 IPs exist in the conflict_set list which states that these two IPs cannot be aliases
bool ips_conflict_exists(binarytree_ip **conflict_set, uint32_t ip1, uint32_t ip2, int ip_pos_1, int ip_pos_2, int* conflict_set_positions) {
    binarytree_ip* ip_tree_1 = NULL;
    binarytree_ip* ip_tree_2 = NULL;

    if (ip_pos_1 != -1)
        ip_tree_1 = find_tree_ip(conflict_set[conflict_set_positions[ip_pos_1]], ip2);

    if (ip_pos_2 != -1)
        ip_tree_2 = find_tree_ip(conflict_set[conflict_set_positions[ip_pos_2]], ip1);

    if (ip_tree_1 == NULL && ip_tree_2 == NULL)
        return false;

    return true;
}

binarytree* get_announcements(binarytree *announcements, char* AS, char const* announcement_filename) {
    FILE *infile = fopen(announcement_filename, "r"); // open the file

    char line[MAXSTRING]; // contains the line
    char *word = NULL;

    while (fgets(line, MAXSTRING, infile) != NULL) { // for each entry
        if (line[0] != '\n') { // if line not empty
            if (line[strlen(line) - 1] == '\n') // if the line ends with \n, remove
                line[strlen(line) - 1] = '\0';

            word = strtok(line, ","); // read AS number

            if ((AS == NULL) || (AS != NULL && strcmp(word, AS) == 0)) { // if the AS is the same as the input
    			word = strtok(NULL, ",");
    			uint32_t token1 = 0;

                if (isValidIp4(word))
                    token1 = convert_ip_to_int(word);
                else
                    token1 = atoi(word);

    			word = strtok(NULL, " \t\r\n\f\v");
    			int token2 = atoi(word);

                announcements = insert_tree(announcements, token1, token2); // add annoucement to the tree
    		}
        }
    }

    fclose(infile); // close the stream
    return announcements;
}

// checks if an IP is within the annoucements of the current AS
bool mask_contains(mask* announcements_array, int announcements_size, uint32_t ip, uint32_t *mask_one, bool tmode) {
    if (tmode)
        return true;

    for (int i = 0; i < announcements_size; i++)
        if (contains(announcements_array[i].data, announcements_array[i].mask, ip, mask_one))
            return true;

    return false;
}

// kapar input version
// generates duple IPs and puts them into a BST
void get_edges(trace* traces, binarytree_unresponsive** unresponsive, char const *file_path, binarytree_duple** aliases, uint32_t *ips_t_array, int size_ips_t, mask* announcements_array, int announcements_size, uint32_t *mask_one, bool verbose, bool tmode, subnet* subnets_array, int subnets_size, int *ips_subnet_id_array, int* reference_point_array, char const *duple_output_filename) {
    int trace_pos = 0;
    FILE *infile = fopen(file_path, "r");
    FILE *outfile = fopen(duple_output_filename, "w");

    if (infile == NULL || outfile == NULL) {
        printf("Error: file pointer is null.\n");
        exit(1);
    }

    // Read file
    char line[MAXSTRING]; // contains the line

    while (fgets(line, MAXSTRING, infile) != NULL) { // for each line in the file
        if (line[0] != '\n' && line[0] != '#') {
            uint32_t ips[500];
            int size = 0;

            do {
                if (line[0] != '\n') {
                    if (line[strlen(line) - 1] == '\n') // if the line ends with \n, remove
                        line[strlen(line) - 1] = '\0';

                    if (!isValidIp4(line)) {
                        printf("ERROR! The IP read is not a real IP! \"%s\"\n", line);
                        exit(1);
                    }

                    ips[size] = convert_ip_to_int(line);
                    size++;
                }
            } while (fgets(line, MAXSTRING, infile) != NULL && line[0] != '#');

            if (traces[trace_pos].stays) {
                int start = traces[trace_pos].start; // inclusive
                int end = traces[trace_pos].end; // inclusive

                /////////////
                // RESOLVE //
                /////////////

                // replace
                int resolve_start = start;
                int resolve_end = start+1;

                while (resolve_start < (end-1)) {
                    while (ip_is_zero(ips[resolve_end]) && resolve_end <= end)
                        resolve_end++;

                    if (resolve_end - resolve_start > 1) {
                        // if not seen, add. skip if already there
                        binarytree_unresponsive* from = find_binarytree_unresponsive(*unresponsive, ips[resolve_start]); // get the last added
                        binarytree_unresponsive_inner* to = find_binarytree_unresponsive_inner(from->inner, ips[resolve_end]); // get the last added

                        int length = to->length;
                        int assign_pos = resolve_start+1; // start changin from here
                        for (int i = 0; i < length; i++) { // as many as the length
                            ips[assign_pos] = (to->ip_start + i);
                            assign_pos++;
                        }
                    }

                    resolve_start = resolve_end;
                    resolve_end = resolve_start+1;
                }

                bool first_ip_is_vp = traces[trace_pos].first_ip_is_vp;

                // add the ips to the tree
                int offset = start;
                if (!first_ip_is_vp)
                    offset++;

                for (int i = offset; i <= (end - 1); i++) { // for each ip (other than VP)
                    uint32_t ip1 = 0;
                    uint32_t ip2 = 0;

                    if (ips[i] < ips[i+1]) {
                        ip1 = ips[i];
                        ip2 = ips[i+1];
                    }
                    else {
                        ip1 = ips[i+1];
                        ip2 = ips[i];
                    }

                    if (ip1 != ip2) { // skip if both IPs are the same
                        int ip1_pos = find(ips_t_array, ip1, size_ips_t);
                        int ip2_pos = find(ips_t_array, ip2, size_ips_t);

                        if (ips_subnet_id_array[ip1_pos] != ips_subnet_id_array[ip2_pos]) { // if the subnet of the IPs are not the same
                            // calculate completeness and reference
                            int left_subnet = -1;
                            int right_subnet = -1;
                            left_subnet = ips_subnet_id_array[ip1_pos];
                            right_subnet = ips_subnet_id_array[ip2_pos];

                            // completeness
                            double a = 1.0;
                            double b = 1.0;

                            if (left_subnet != -1) // if found
                                a = subnets_array[left_subnet].completeness;
                            if (right_subnet != -1) // if found
                                b = subnets_array[right_subnet].completeness;
                            double completeness = (a*b) / (a+b);

                            // reference
                            a = 1.0;
                            b = 1.0;

                            if (left_subnet != -1) // if found
                                a = reference_point_array[ip1_pos];
                            if (right_subnet != -1) // if found
                                b = reference_point_array[ip2_pos];
                            double reference = (a*b) / (a+b);

                            *aliases = insert_tree_with_ip_duple(*aliases, ip1, ip2, ip1_pos, ip2_pos, completeness, reference); // insert duple
                            fprintf(outfile, "%s %s\n", convert_int_to_ip(ip1), convert_int_to_ip(ip2));
                        }
                    }
                }
            }

            trace_pos++;
        }
    }

    fclose(infile);
    fclose(outfile);
}

// kapar input version
// return IPs, VPs, Ingresses and Egresses in binary trees
void get_ips_vps(trace* traces, binarytree_unresponsive** unresponsive, char const *file_path, binarytree_ip** vps_ingress, binarytree_ip** vps_egress, binarytree_ip** vps_vp, binarytree_ip** ips_t, binarytree_ip** ips_traced, mask* announcements_array, int announcements_size, uint32_t *mask_one, bool verbose, bool tmode, int no_of_traces, binarytree_ip* ground_truth_ips, char const* modified_traces_filename, bool write_traces, char const* midar_filename) {
    int zero_ip_counter = 1;
    int trace_pos = 0;
    FILE* infile = fopen(file_path, "r");
    FILE* modified_traces = NULL;
    if (write_traces)
        modified_traces = fopen(modified_traces_filename, "w");
    int max_no_of_ips_in_a_trace = 1000;

    if (infile == NULL) {
        printf("Error: file pointer is null.\n");
        exit(1);
    }

    // Read file
    char line[MAXSTRING]; // contains the line
    char *word = NULL;

    while (fgets(line, MAXSTRING, infile) != NULL) { // for each trace in the file
        // if this is the line containing the starting IP
        if (line[0] != '\n' && line[0] != '#') {
            //if ((trace_pos+1) % 1000 == 0)
                //printf("%d/%d/%d\r", trace_pos+1, no_of_traces, zero_ip_counter);

            uint32_t* ips = (uint32_t*) malloc(sizeof(uint32_t) * max_no_of_ips_in_a_trace);
            int size = 0; // both position and the number of IPs

            // read all the IPs of the trace
            do {
                if (line[0] != '\n') {
                    if (line[strlen(line) - 1] == '\n') // if the line ends with \n, remove
                        line[strlen(line) - 1] = '\0';

                    if (!isValidIp4(line)) {
                        printf("ERROR! The IP read is not a real IP! \"%s\"\n", line);
                        exit(1);
                    }

                    ips[size] = convert_ip_to_int(line);
                    size++;

                    if (size >= max_no_of_ips_in_a_trace) {
                        printf("ERROR! Exceeds the array.\n");
                        exit(0);
                    }
                }
            } while (fgets(line, MAXSTRING, infile) != NULL && line[0] != '#');

            int start = 0; // inclusive
            int end = (size-1); // inclusive
            int temp_start = -1; // inclusive
            int temp_end = -1; // inclusive

            //////////////
            // TRIM 0's //
            //////////////
            temp_start = start; // inclusive
            temp_end = end; // inclusive

            // trim from the beginning
            for (int i = temp_start; i <= temp_end; i++) {
                if (!ip_is_zero(ips[i]) &&
                    (mask_contains(announcements_array, announcements_size, ips[i], mask_one, tmode) || mask_contains(announcements_array, announcements_size, ips[i+1], mask_one, tmode)) &&
                    ((ground_truth_ips == NULL) || (ground_truth_ips != NULL && (find_tree_ip(ground_truth_ips, ips[i]) || find_tree_ip(ground_truth_ips, ips[i+1])))))
                    break;
                else
                    start++;
            }

            // trim from the end
            for (int i = temp_end; i >= temp_start; i--) {
                if (!ip_is_zero(ips[i]) &&
                    mask_contains(announcements_array, announcements_size, ips[i], mask_one, tmode) &&
                    ((ground_truth_ips == NULL) || (ground_truth_ips != NULL && (find_tree_ip(ground_truth_ips, ips[i])))))
                    break;
                else
                    end--;
            }
            /////////
            // END //
            /////////

            // if trace is within boundary
            if ((end-start) >= 1 && end > 0 && start < (size-1)) {
                // cut loops (ignore the first IP since it is an identifier)
                for (int i = end; i > (start+1); i--) {
                    for (int j = (i - 1); j >= (start+1); j--) {
                        if (ips[i] == ips[j]) {
                            end = (i-1);
                            break;
                        }
                    }
                }

                // ****** trim from the end AGAIN because the last ip after cutting may be 0
                //////////////
                // TRIM 0's //
                //////////////
                temp_start = start; // inclusive
                temp_end = end; // inclusive

                // trim from the end
                for (int i = temp_end; i >= temp_start; i--) {
                    if (!ip_is_zero(ips[i]) &&
                        mask_contains(announcements_array, announcements_size, ips[i], mask_one, tmode) &&
                        ((ground_truth_ips == NULL) || (ground_truth_ips != NULL && (find_tree_ip(ground_truth_ips, ips[i])))))
                        break;
                    else
                        end--;
                }
                /////////
                // END //
                /////////

                // if there are at least 2 IPs in the trace
                if ((end-start) >= 1 && end > 0 && start < (size-1)) {
                    // if there are at least 4 IPs, check for bounce back
                    if ((end-start) >= 3) {
                        for (int i = start+1; i < (end-1); i++) {
                            int bounce_start = i;
                            int bounce_end = i+1;
                            int wave_match = 0;

                            while (bounce_start >= start && bounce_end <= end && ((!ip_is_zero(ips[bounce_start]) && ip_is_zero(ips[bounce_end])) || (ip_is_zero(ips[bounce_start]) && !ip_is_zero(ips[bounce_end])) || (!ip_is_zero(ips[bounce_start]) && !ip_is_zero(ips[bounce_end]) && get_abs_uint32(ips[bounce_start], ips[bounce_end]) == 1))) {
                                if (!ip_is_zero(ips[bounce_start]) && !ip_is_zero(ips[bounce_end]) && get_abs_uint32(ips[bounce_start], ips[bounce_end]) == 1)
                                    wave_match++;

                                if (wave_match >= 2)
                                    break; // cut wave check

                                bounce_start--;
                                bounce_end++;
                            }

                            if (wave_match >= 2) {
                                if (ip_is_zero(ips[i]))
                                    end = i-1;
                                else
                                    end = i;
                                break;
                            }
                        }

                        // ****** trim from the end AGAIN because the last ip after cutting may be 0
                        //////////////
                        // TRIM 0's //
                        //////////////
                        temp_start = start; // inclusive
                        temp_end = end; // inclusive

                        // trim from the end
                        for (int i = temp_end; i >= temp_start; i--) {
                            if (!ip_is_zero(ips[i]) &&
                                mask_contains(announcements_array, announcements_size, ips[i], mask_one, tmode) &&
                                ((ground_truth_ips == NULL) || (ground_truth_ips != NULL && (find_tree_ip(ground_truth_ips, ips[i])))))
                                break;
                            else
                                end--;
                        }
                        /////////
                        // END //
                        /////////

                        // check for the case of [* * ip] where number of *'s is greater than equal to 2
                        // if there are at least 3 IPs, check for bounce back
                        if (!ip_is_zero(ips[end]) && ip_is_zero(ips[end-1]) && ip_is_zero(ips[end-2])) {
                            end--;
                            while (end > start && ip_is_zero(ips[end]))
                                end--;
                        }
                    }

                    // if there are at least 2 IPs in the trace
                    if ((end-start) >= 1 && end > 0 && start < (size-1)) {
                        traces[trace_pos].stays = true;

                        // check if egress will be considered
                        // if no loops,
                        // if the last IP is not 0,
                        // if the last IP is not in the subnet annoucements, THEN check egress
                        bool last_ip_is_vp = false;
                        if ((end == (size-1)) && !mask_contains(announcements_array, announcements_size, ips[end], mask_one, tmode) && !ip_is_zero(ips[end]))
                            last_ip_is_vp = true;

                        traces[trace_pos].last_ip_is_vp = last_ip_is_vp;

                        // add the ip at the end
                        if (end == size-1)
                            *ips_traced = insert_tree_ip(*ips_traced, ips[end]);

                        //////////////////////////////////////////////////////////////
                        // RESOLVE

                        // resolve 0 IPS
                        int resolve_start = start;
                        int resolve_end = start+1;

                        while (resolve_start < (end-1)) {
                            while (ip_is_zero(ips[resolve_end]) && resolve_end <= end)
                                resolve_end++;

                            // if there are 0 ips in the trace
                            if (resolve_end - resolve_start > 1) {
                                // if not seen, add. skip if already there
                                *unresponsive = insert_binarytree_unresponsive(*unresponsive, ips[resolve_start]); // add
                                binarytree_unresponsive* from = find_binarytree_unresponsive(*unresponsive, ips[resolve_start]); // get the last added

                                if (from == NULL) {
                                    printf("Couldn't find resolution!\n");
                                    exit(0);
                                }

                                int length = (resolve_end - resolve_start) - 1;
                                from->inner = insert_binarytree_unresponsive_inner(from->inner, ips[resolve_end], zero_ip_counter, length);
                                //binarytree_unresponsive_inner* to = find_binarytree_unresponsive_inner(from->inner, ips[resolve_end]); // get the last added

                                // for each 0 ip in the range
                                for (int i = 0; i < length; i++)
                                    zero_ip_counter++;
                            }

                            resolve_start = resolve_end;
                            resolve_end = resolve_start+1;
                        }

                        // replace 0's with resolved ips
                        resolve_start = start;
                        resolve_end = start+1;

                        while (resolve_start < (end-1)) {
                            while (ip_is_zero(ips[resolve_end]) && resolve_end <= end)
                                resolve_end++;

                            if (resolve_end - resolve_start > 1) {
                                binarytree_unresponsive* from = find_binarytree_unresponsive(*unresponsive, ips[resolve_start]); // get the last added
                                binarytree_unresponsive_inner* to = find_binarytree_unresponsive_inner(from->inner, ips[resolve_end]); // get the last added

                                if (from == NULL || to == NULL) {
                                    printf("Couldn't find resolution!\n");
                                    exit(0);
                                }

                                int length = to->length;
                                for (int i = 0; i < length; i++) // as many as the length
                                    ips[resolve_start + i + 1] = (to->ip_start + i);
                            }

                            resolve_start = resolve_end;
                            resolve_end = resolve_start+1;
                        }
                        //////////////////////////////////////////////////////////////

                        if (write_traces) {
                            // write modified traces to file
                            fprintf(modified_traces, "#\n");
                            for (int i = start; i <= end; i++) // for each IP
                                fprintf(modified_traces, "%s\n", convert_int_to_ip(ips[i]));
                            fprintf(modified_traces, "#\n");
                        }

                        if (verbose) {
                            for (int i = start; i <= end; i++) // for each IP
                                printf("%s -> ", convert_int_to_ip(ips[i]));
                            printf("\n");
                        }

                        //////////////////////////////////////////////////////////////

                        traces[trace_pos].start = start;
                        traces[trace_pos].end = end;

                        // add vps/ingress to the trees
                        // check if the first ip is Ingress or VP
                        bool first_ip_is_vp = mask_contains(announcements_array, announcements_size, ips[start], mask_one, tmode);
                        traces[trace_pos].first_ip_is_vp = first_ip_is_vp;

                        if (first_ip_is_vp)
                            *vps_vp = insert_tree_ip(*vps_vp, ips[start]); // Fill VP tree
                        else
                            *vps_ingress = insert_tree_ip(*vps_ingress, ips[start]); // Fill ingress tree

                        // add the ips to the tree
                        int offset = start;
                        if (!first_ip_is_vp)
                            offset++;

                        for (int i = offset; i <= end; i++) // for each ip
                            *ips_t = insert_tree_ip(*ips_t, ips[i]);

                        // add egress to the tree
                        if (last_ip_is_vp)
                            *vps_egress = insert_tree_ip(*vps_egress, ips[end]);
                    }
                    else
                        traces[trace_pos].stays = false;
                }
                else
                    traces[trace_pos].stays = false;
            }
            else
                traces[trace_pos].stays = false;

            trace_pos++;
            free(ips);
        }
    }

    fclose(infile);

    if (write_traces)
        fclose(modified_traces);


    ////////////////////
    // Load midar IPs //
    ////////////////////
    if (midar_filename != NULL) { // if midar file is provided
        FILE* midar_infile = fopen(midar_filename, "r"); // open the file

        if (midar_infile == NULL) {
            printf("WARNING! Midar file pointer is null, skipping!\n");
            fprintf(midar_infile, "WARNING! Midar file pointer is null, skipping!\n");
        }
        else {
            // read MIDAR IPs
            while (fgets(line, MAXSTRING, midar_infile) != NULL) { // for each entry
                line[strlen(line) - 1] = '\0';
                if (line[0] != '#' && line[0] != '\n') {
                    word = strtok(line, " \t\r\n\f\v"); // ip number
                    *ips_t = insert_tree_ip(*ips_t, convert_ip_to_int(word));

                    word = strtok(NULL, " \t\r\n\f\v"); // ip number
                    *ips_t = insert_tree_ip(*ips_t, convert_ip_to_int(word));
                }
            }
            fclose(midar_infile);
        }
    }
    /////////
    // END //
    /////////
}

int check_special_ips(int start, int end, bool verbose, int separators_size, separators_struct* separators, uint32_t *ips_t_array, uint32_t *mask_one, int size_ips_t, uint32_t *mask_zero, bool split) {
    bool split_occurs = false;

    // get the mask
    int mask = get_mask_matches(start, end, mask_one, ips_t_array);

    if (verbose)
        printf("Block start: %d, end: %d\n", start, end);

    if ((is_special(ips_t_array[start], mask, mask_zero)) || (is_special(ips_t_array[end], mask, mask_zero))) {
        if (verbose)
            printf("Found special IP...\n");

        while (mask>0 && ((is_special(ips_t_array[start], mask, mask_zero)) || (is_special(ips_t_array[end], mask, mask_zero)))) {
            if(verbose) {
                if (is_special(ips_t_array[start], mask, mask_zero))
                    printf("start ip is special\n");
                if (is_special(ips_t_array[end], mask, mask_zero))
                    printf("end ip is special\n");
            }

            mask--;

            if (verbose)
                printf("Subtracting 1 from mask... New mask %d\n", mask);
        }

        if (verbose)
            printf("Stopped at mask: %d\n", mask);

        if (split) {
            if ((start == 0 && contains(ips_t_array[end], mask, ips_t_array[end+1], mask_one)) ||
                (end == (size_ips_t-1) && contains(ips_t_array[start], mask, ips_t_array[start-1], mask_one)) ||
                    contains(ips_t_array[end], mask, ips_t_array[end+1], mask_one) ||
                    contains(ips_t_array[start], mask, ips_t_array[start-1], mask_one)) {
                if (verbose)
                    printf("Splitting since the new mask covers the IPs before or after.\n");

                int mask_to_match = get_mask_matches(start, end, mask_one, ips_t_array);
                mask_to_match++;
                for (int i = start+1; i <= end; i++) { // for each ip other than 0 since that is what we are comparing with
                    int mask_temp = get_mask_matches(start, i, mask_one, ips_t_array);
                    if (mask_temp < mask_to_match) {
                        split_occurs = true;
                        //printf("SPLIT 1\n");
                        separators[i-1].state = false;

                        if (verbose)
                            printf("\tChanging the state to FALSE at %d!\n", i-1);

                        break;
                    }
                }
            }
            else if (verbose)
                printf("The new subnet does not cover the IP before or after so no splitting\n");
        }
    }
    else if (verbose)
        printf("No special ips, skipping!\n");

    if (split_occurs)
        return -1;

    return mask;
}

void fill_distances(trace* traces, binarytree_unresponsive** unresponsive, binarytree_ip **conflict_set_subnet, binarytree_ip **conflict_set, char const *file_path, uint32_t *ips_t_array, int size_ips_t, mask* announcements_array, int announcements_size, uint32_t *mask_one, uint32_t *vps_vp_array, int size_vps_vp, short **vps, uint32_t *vps_ingress_array, int size_vps_ingress, short **ingress, uint32_t *vps_egress_array, int size_vps_egress, short **egress, bool tmode, bool verbose, linkedlist **neighbors) {
    int zero_ip_counter = 1;
    int trace_pos = 0;
    FILE *infile = fopen(file_path, "r");

    if (infile == NULL) {
        printf("Error: file pointer is null.\n");
        exit(1);
    }

    // Read file
    char line[MAXSTRING]; // contains the line

    while (fgets(line, MAXSTRING, infile) != NULL) { // for each line in the file
        if (line[0] != '\n' && line[0] != '#') {
            uint32_t ips[500];
            int size = 0;

            // read the IPs until '#' character
            do {
                if (line[0] != '\n') {
                    if (line[strlen(line) - 1] == '\n') // if the line ends with \n, remove
                        line[strlen(line) - 1] = '\0';

                    if (!isValidIp4(line)) {
                        printf("ERROR! The IP read is not a real IP! \"%s\"\n", line);
                        exit(1);
                    }

                    ips[size] = convert_ip_to_int(line);
                    size++;
                }
            } while (fgets(line, MAXSTRING, infile) != NULL && line[0] != '#');


            if (traces[trace_pos].stays) {
                int start = traces[trace_pos].start; // inclusive
                int end = traces[trace_pos].end; // inclusive


                //RESOLVE

                // replace
                int resolve_start = start;
                int resolve_end = start+1;

                while (resolve_start < (end-1)) {
                    while (ip_is_zero(ips[resolve_end]) && resolve_end <= end)
                        resolve_end++;

                    if (resolve_end - resolve_start > 1) {
                        // if not seen, add. skip if already there
                        binarytree_unresponsive* from = find_binarytree_unresponsive(*unresponsive, ips[resolve_start]); // get the last added
                        binarytree_unresponsive_inner* to = find_binarytree_unresponsive_inner(from->inner, ips[resolve_end]); // get the last added

                        int length = to->length;
                        int assign_pos = resolve_start+1; // start changin from here
                        for (int i = 0; i < length; i++) { // as many as the length
                            ips[assign_pos] = (to->ip_start + i);
                            assign_pos++;
                        }
                    }

                    resolve_start = resolve_end;
                    resolve_end = resolve_start+1;
                }




                if (verbose) {
                    for (int i = start; i <= end; i++) // for each IP
                        printf("%s -> ", convert_int_to_ip(ips[i]));
                    printf("\n");
                }

                /////////////////////////////////////////////////////////////////////

                bool first_ip_is_vp = traces[trace_pos].first_ip_is_vp;
                bool last_ip_is_vp = traces[trace_pos].last_ip_is_vp;

                // Generate the conflict set for the current trace
                int conf_start = start;
                if (!first_ip_is_vp)
                    conf_start++;

                for (int i = conf_start; i <= end; i++) { // for each ip
                    // find the positon of the IP at position i in the IPs list (to find the position of the BST which contains the conflict set)
                    int ip_pos = find(ips_t_array, ips[i], size_ips_t);

                    if (ip_pos != -1) {
                        for (int j = conf_start; j <= end; j++) { // for each ip
                            if (abs(i-j) >= 2)
                                conflict_set_subnet[ip_pos] = insert_tree_ip(conflict_set_subnet[ip_pos], ips[j]);

                            if (abs(i-j) >= 1)
                                conflict_set[ip_pos] = insert_tree_ip(conflict_set[ip_pos], ips[j]);

                            if (abs(i-j) == 1) {
                                int ip_pos2 = find(ips_t_array, ips[j], size_ips_t);
                                neighbors[ip_pos] = insert_list_alias(neighbors[ip_pos], ips[j], ip_pos2);
                            }
                        }
                    }
                }

                /////////////////////////////////////////////////////////////////////

                // check if the first ip is Ingress or VP
                if (first_ip_is_vp) { // it is VP
                    int vp_pos = find(vps_vp_array, ips[start], size_vps_vp);
                    if (vp_pos != -1) {
                        // add the arrays to the tree
                        for (int i = start; i <= end; i++) { // for each ip
                            int ip_pos = find(ips_t_array, ips[i], size_ips_t);

                            // check if the hop number exists in the linkedlist
                            if (ip_pos != -1)
                                vps[vp_pos][ip_pos] = min(vps[vp_pos][ip_pos], i-start); // add hop to temp
                        }
                    }
                }
                else { // it is Ingress
                    int vp_pos = find(vps_ingress_array, ips[start], size_vps_ingress);
                    if (vp_pos != -1) {
                        // add the arrays to the tree
                        for (int i = start; i <= end; i++) { // for each ip (other than VP)
                            int ip_pos = find(ips_t_array, ips[i], size_ips_t);

                            // check if the hop number exists in the linkedlist
                            if (ip_pos != -1)
                                ingress[vp_pos][ip_pos] = min(ingress[vp_pos][ip_pos], i-start); // add hop to temp
                        }
                    }
                }

                // Fill egress tree if there are no loops
                if (last_ip_is_vp) {
                    // add the VP to root tree
                    int vp_pos = find(vps_egress_array, ips[end], size_vps_egress);
                    if (vp_pos != -1) {
                        // add the arrays to the tree
                        for (int i = end; i >= start ; i--) {
                            int ip_pos = find(ips_t_array, ips[i], size_ips_t);

                            if (ip_pos == -1)
                                egress[vp_pos][ip_pos] = min(egress[vp_pos][ip_pos], end - i); // add hop to temp
                        }
                    }
                }
            }

            trace_pos++;
        }
    }
}

double check_if_alias_distance_matches(uint32_t IP1, uint32_t IP2, short **ingress, int size_vps_ingress, short **egress, int size_vps_egress, short **vps, int size_vps_vp, uint32_t *ips_t_array, int size_ips_t, int IP1_pos, int IP2_pos) {
    if (IP1_pos == -1 || IP2_pos == -1) {
        printf("Couldn't find the IP in the list. This shouldn't happen. Check. Breaking!!!\n");
        exit(1);
    }

    int observed = 0;
    int match = 0;

    for (int i = 0; i < size_vps_vp; i++) {
        if (!(vps[i][IP1_pos] == 255 && vps[i][IP2_pos] == 255)) {
            observed++;

            if (vps[i][IP1_pos] == 255 || vps[i][IP2_pos] == 255 || vps[i][IP1_pos] == vps[i][IP2_pos])
                match++;
        }
    }

    for (int i = 0; i < size_vps_ingress; i++) {
        if (!(ingress[i][IP1_pos] == 255 && ingress[i][IP2_pos] == 255)) {
            observed++;

            if (ingress[i][IP1_pos] == 255 || ingress[i][IP2_pos] == 255 || ingress[i][IP1_pos] == ingress[i][IP2_pos])
                match++;
        }
    }

    for (int i = 0; i < size_vps_egress; i++) {
        if (!(egress[i][IP1_pos] == 255 && egress[i][IP2_pos] == 255)) {
            observed++;

            if (egress[i][IP1_pos] == 255 || egress[i][IP2_pos] == 255 || egress[i][IP1_pos] == egress[i][IP2_pos])
                match++;
        }
    }

    if (observed == 0)
        return 1.0;

    return ((double) match / (double) observed);
}

// get the positions of IPs for min and max distance (range)
int* find_in_all(uint32_t *ips_t_array, int size_ips_t, binarytree_ip **conflict_set, int *conflict_set_positions, short **ingress, int size_vps_ingress, short **egress, int size_vps_egress, short **vps, int size_vps_vp, int start, int end, double subnet_group_error_threshold, int mode) {
    int *result = (int*) malloc(sizeof(int) * 2);
    result[0] = -1;
    result[1] = -1;

    if (mode == 1 || mode == 3) {
        int no_of_non_matches = 0;
        int total = 0;

        // first calculate the error rate
        for (int i = 0; i < size_vps_ingress; i++) { // for each ingress
            int current_min = 255;
            int current_max = 0;
            int pos_min = -1;
            int pos_max = -1;

            bool consider = false;
            for (int j = start; j <= end; j++) { // for each IP
                if (ingress[i][j] != 255) { // if not null
                    consider = true;

                    // check min
                    if (ingress[i][j] < current_min) {
                        current_min = ingress[i][j];
                        pos_min = j;
                    }

                    // check max
                    if (ingress[i][j] > current_max) {
                        current_max = ingress[i][j];
                        pos_max = j;
                    }
                }
            }

            if (consider)
                total++;

            if ((current_max - current_min) > 1) {
                no_of_non_matches++;

                if (result[0] == -1 && result[1] == -1) {
                    result[0] = pos_min;
                    result[1] = pos_max;
                }
            }
        }

        for (int i = 0; i < size_vps_egress; i++) { // for each egress
            int current_min = 255;
            int current_max = 0;
            int pos_min = -1;
            int pos_max = -1;

            bool consider = false;
            for (int j = start; j <= end; j++) { // for each IP
                if (egress[i][j]!= 255) { // if not null
                    consider = true;

                    // check min
                    if (egress[i][j] < current_min) {
                        current_min = egress[i][j];
                        pos_min = j;
                    }

                    // check max
                    if (egress[i][j] > current_max) {
                        current_max = egress[i][j];
                        pos_max = j;
                    }
                }
            }

            if (consider)
                total++;

            if ((current_max - current_min) > 1) {
                no_of_non_matches++;

                if (result[0] == -1 && result[1] == -1) {
                    result[0] = pos_min;
                    result[1] = pos_max;
                }
            }
        }

        for (int i = 0; i < size_vps_vp; i++) { // for each vp
            int current_min = 255;
            int current_max = 0;
            int pos_min = -1;
            int pos_max = -1;

            bool consider = false;
            for (int j = start; j <= end; j++) { // for each IP
                if (vps[i][j]!= 255) { // if not null
                    consider = true;

                    // check min
                    if (vps[i][j] < current_min) {
                        current_min = vps[i][j];
                        pos_min = j;
                    }

                    // check max
                    if (vps[i][j] > current_max) {
                        current_max = vps[i][j];
                        pos_max = j;
                    }
                }
            }

            if (consider)
                total++;

            if ((current_max - current_min) > 1) {
                no_of_non_matches++;

                if (result[0] == -1 && result[1] == -1) {
                    result[0] = pos_min;
                    result[1] = pos_max;
                }
            }
        }

        // if the error rate is below the threshold, then return the default values for result meaning the group is fine
        if (((double)(no_of_non_matches) / (double)(total)) <= subnet_group_error_threshold) {
            result[0] = -1;
            result[1] = -1;
        }
    }

    if (mode == 2 || mode == 3) {
        for (int i = start; i < end; i++) { // for each IP
            for (int j = i+1; j <= end; j++) { // for each IP
                if (ips_conflict_exists(conflict_set, ips_t_array[i], ips_t_array[j], i, j, conflict_set_positions)) {
                    result[0] = i;
                    result[1] = j;
                }
            }
        }
    }

    return result;
}

void compare_group_distances(binarytree_ip **conflict_set, int *conflict_set_positions, bool verbose, int separators_size, separators_struct* separators, short **ingress, short **egress, short **vps, int size_vps_ingress, int size_vps_egress, int size_vps_vp, uint32_t *ips_t_array, uint32_t *mask_one, uint32_t *mask_zero, int size_ips_t, double subnet_group_error_threshold, int mode) {
    int start = 0;
    int end = 0;

    while (start < separators_size) {
        if (separators[start].state) { // if true
            end = find_end(separators, separators_size, start);

            // get the masks of the two IPs
            int mask = check_special_ips(start, end, verbose, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, true);

            if (mask != -1) { // so that it would skip if split had occured
                // check for special IPs here since you never checked before. But in the other functions, check only after you split
                bool split_occurs = false;

                int* result = find_in_all(ips_t_array, size_ips_t, conflict_set, conflict_set_positions, ingress, size_vps_ingress, egress, size_vps_egress, vps, size_vps_vp, start, end, subnet_group_error_threshold, mode);
                int pos_min = result[0];
                int pos_max = result[1];

                if ((pos_min != -1) && (pos_max != -1)) { // if there is conflict
                    if (verbose) {
                        printf("Block start: %d, end: %d\n", start, end);
                        printf("\tpos_max: %d, pos_min: %d\n", pos_max, pos_min);
                        printf("\tIPs: %s, %s\n", convert_int_to_ip(ips_t_array[pos_max]), convert_int_to_ip(ips_t_array[pos_min]));
                        printf("\tMask between start and end: %d\n\n", mask);
                    }

                    // get the masks of the two IPs
                    //mask = get_mask_matches(pos_min, pos_max, mask_one, ips_t_array);
                    mask = check_special_ips(pos_min, pos_max, false, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, false);

                    if (verbose)
                        printf("\tMask between pos_min and pos_max: %d\n\n", mask);

                    int mask_to_match = mask + 1;

                    if (verbose)
                        printf("\tMask to match between pos_min and pos_max: %d\n\n", mask_to_match);

                    int from = max(pos_min, pos_max);

                    if (verbose)
                        printf("\tfrom: %d\n\n", from);

                    // upwards

                    if(verbose)
                        printf("\tUPWARDS:\n\n");

                    int to = from-1;

                    while (to >= min(pos_min, pos_max)) {
                        //int mask_temp = get_mask_matches(from, to, mask_one, ips_t_array);
                        int mask_temp = check_special_ips(from, to, false, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, false);

                        if (verbose) {
                            printf("\tcomparing %d with %d\n", from, to);
                            printf("\tcurrent mask is:%d\n", mask_temp);
                        }

                        if (mask_temp < mask_to_match) {
                            if (verbose)
                                printf("\tchanging separator from position: %d to FALSE\n", to);

                            //printf("SPLIT 3\n");
                            separators[to].state = false;
                            break;
                        }

                        to--;

                        if (verbose)
                            printf("\n");
                    }

                    from = to;
                    to--;

                    while (to >= start) {
                        //int mask_temp = get_mask_matches(from, to, mask_one, ips_t_array);
                        int mask_temp = check_special_ips(from, to, false, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, false);

                        if (verbose) {
                            printf("\tcomparing %d with %d\n", from, to);
                            printf("\tcurrent mask is:%d\n", mask_temp);
                        }

                        if (mask_temp < mask_to_match) {
                            if (verbose)
                                printf("\tchanging separator from position: %d to FALSE\n", to);

                            //printf("SPLIT 4\n");
                            separators[to].state = false;
                            break;
                        }

                        to--;

                        if (verbose)
                            printf("\n");
                    }

                    // downwards
                    if (verbose) {
                        printf("\n");
                        printf("\tDOWNWARDS:\n\n");
                    }

                    from = max(pos_min, pos_max);
                    to = from+1;

                    while (to <= end) {
                        //int mask_temp = get_mask_matches(from, to, mask_one, ips_t_array);
                        int mask_temp = check_special_ips(from, to, false, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, false);

                        if (verbose) {
                            printf("\tcomparing %d with %d\n", from, to);
                            printf("\tcurrent mask is:%d\n", mask_temp);
                        }

                        if (mask_temp < mask_to_match) {
                            if (verbose)
                                printf("\tchanging separator from position: %d to FALSE\n", to-1);

                            //printf("SPLIT 5\n");
                            separators[to-1].state = false;
                            break;
                        }

                        to++;
                    }

                    if (verbose)
                        printf("\n");
                }
                else {
                    if (verbose)
                        printf("Skipping positions: %d-%d since there is no conflict in distances\n\n", start, end);
                    start = end + 1;
                }
            }
            else if (verbose)
                printf("Splitting occured during special IP check, going back...\n");
        }
        else
            start++;
    }
}

void check_completeness(bool verbose, int separators_size, separators_struct* separators, uint32_t *ips_t_array, uint32_t *mask_one, int size_ips_t, double completeness_threshold, uint32_t *mask_zero) {
    int start = 0;
    int end = 0;

    while (start < separators_size) { // for each subnet
        if (separators[start].state) { // if true
            end = find_end(separators, separators_size, start);

            // get the mask
            int mask = check_special_ips(start, end, verbose, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, true);

            if (mask != -1) { // so that it would skip if split had occured
                bool split_occurs = false;

                if (mask < 30) {
                    // check completeness
                    int no_of_ips = (end - start) + 1;
                    int total_ips_in_subnet = pow(2, (32 - mask)) - 2;
                    double completeness = (1 - ((double) no_of_ips / (double) total_ips_in_subnet));

                    if (verbose) {
                        printf("Block start: %d, end: %d\n", start, end);
                        printf("\tNo of ips between start and end: %d\n", no_of_ips);
                        printf("\tTotal no of ips in the subnet: %d\n\n", total_ips_in_subnet);
                        printf("\tComparing (completenes error w/ threshold): %f - %f\n\n", completeness, completeness_threshold);
                    }

                    if (completeness > completeness_threshold) { // doesn't pass the threshold
                        int mask_to_match = mask+1;

                        if (verbose) {
                            printf("\tDoesn't pass the threshold\n");
                            printf("\tMask between start and end is: %d\n", mask);
                            printf("\tMask to match is: %d\n", mask_to_match);
                        }

                        // split
                        for (int i = start+1; i <= end; i++) { // for each ip other than 0 since that is what we are comparing with
                            //int mask_temp = get_mask_matches(start, i, mask_one, ips_t_array);
                            int mask_temp = check_special_ips(start, i, false, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, false);
                            if (mask_temp < mask_to_match) {
                                split_occurs = true;
                                //printf("SPLIT 2\n");
                                separators[i-1].state = false;

                                if (verbose)
                                    printf("\tChanging the state to FALSE at %d!\n", i-1);

                                break;
                            }
                        }
                    }
                    else if (verbose)
                        printf("\tPasses the threshold, skipping!\n");
                }

                if (!split_occurs)
                    start = end+1;
            }
            else if (verbose)
                printf("Splitting occured during special IP check, going back...\n");
        }
        else
            start++;

        if (verbose)
            printf("\n");
    }
}

/*
void DFS(completeness_vp_struct largest_vp_pos, binarytree_seen **visited, binarytree **ips, short **ingress, short **egress, short **vps, int size_vps_ingress, int size_vps_egress, int size_vps_vp, int start, int end, int no_of_ips, uint32_t *ips_t_array) {
    for (int i = start; i <= end; i++) { // for each IP in VP
        if (largest_vp_pos.type == 0) { // for VP
            if (vps[largest_vp_pos.vp][i] != 255) {
                *ips = insert_tree(*ips, ips_t_array[i], -1);
                *visited = insert_tree_seen(*visited, largest_vp_pos.type, largest_vp_pos.vp, i);

                for (int j = 0; j < size_vps_vp; j++) { // for each VP
                    if (vps[j][i] != 255 && find_tree_seen(*visited, 0, j, i) == NULL) {
                        *ips = insert_tree(*ips, ips_t_array[i], -1);
                        *visited = insert_tree_seen(*visited, 0, j, i);
                        completeness_vp_struct temp;
                        temp.type = 0;
                        temp.vp = j;
                        DFS(temp, visited, ips, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, no_of_ips, ips_t_array);
                    }
                }

                for (int j = 0; j < size_vps_ingress; j++) { // for each Ingress
                    if (ingress[j][i] != 255 && find_tree_seen(*visited, 1, j, i) == NULL) {
                        *ips = insert_tree(*ips, ips_t_array[i], -1);
                        *visited = insert_tree_seen(*visited, 1, j, i);
                        completeness_vp_struct temp;
                        temp.type = 1;
                        temp.vp = j;
                        DFS(temp, visited, ips, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, no_of_ips, ips_t_array);
                    }
                }

                for (int j = 0; j < size_vps_egress; j++) { // for each Egress
                    if (egress[j][i] != 255 && find_tree_seen(*visited, 2, j, i) == NULL) {
                        *ips = insert_tree(*ips, ips_t_array[i], -1);
                        *visited = insert_tree_seen(*visited, 2, j, i);
                        completeness_vp_struct temp;
                        temp.type = 2;
                        temp.vp = j;
                        DFS(temp, visited, ips, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, no_of_ips, ips_t_array);
                    }
                }
            }
        }
        else if (largest_vp_pos.type == 1) { // for Ingress
            if (ingress[largest_vp_pos.vp][i] != 255) {
                *ips = insert_tree(*ips, ips_t_array[i], -1);
                *visited = insert_tree_seen(*visited, largest_vp_pos.type, largest_vp_pos.vp, i);

                for (int j = 0; j < size_vps_vp; j++) { // for each VP
                    if (vps[j][i] != 255) {
                        if (find_tree_seen(*visited, 0, j, i) == NULL) {
                            *ips = insert_tree(*ips, ips_t_array[i], -1);
                            *visited = insert_tree_seen(*visited, 0, j, i);
                            completeness_vp_struct temp;
                            temp.type = 0;
                            temp.vp = j;
                            DFS(temp, visited, ips, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, no_of_ips, ips_t_array);
                        }
                    }
                }

                for (int j = 0; j < size_vps_ingress; j++) { // for each Ingress
                    if (ingress[j][i] != 255) {
                        if (find_tree_seen(*visited, 1, j, i) == NULL) {
                            *ips = insert_tree(*ips, ips_t_array[i], -1);
                            *visited = insert_tree_seen(*visited, 1, j, i);
                            completeness_vp_struct temp;
                            temp.type = 1;
                            temp.vp = j;
                            DFS(temp, visited, ips, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, no_of_ips, ips_t_array);
                        }
                    }
                }

                for (int j = 0; j < size_vps_egress; j++) { // for each Egress
                    if (egress[j][i] != 255) {
                        if (find_tree_seen(*visited, 2, j, i) == NULL) {
                            *ips = insert_tree(*ips, ips_t_array[i], -1);
                            *visited = insert_tree_seen(*visited, 2, j, i);
                            completeness_vp_struct temp;
                            temp.type = 2;
                            temp.vp = j;
                            DFS(temp, visited, ips, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, no_of_ips, ips_t_array);
                        }
                    }
                }
            }
        }
        else if (largest_vp_pos.type == 2) { // for Egress
            if (egress[largest_vp_pos.vp][i] != 255) {
                *ips = insert_tree(*ips, ips_t_array[i], -1);
                *visited = insert_tree_seen(*visited, largest_vp_pos.type, largest_vp_pos.vp, i);

                for (int j = 0; j < size_vps_vp; j++) { // for each VP
                    if (vps[j][i] != 255) {
                        if (find_tree_seen(*visited, 0, j, i) == NULL) {
                            *ips = insert_tree(*ips, ips_t_array[i], -1);
                            *visited = insert_tree_seen(*visited, 0, j, i);
                            completeness_vp_struct temp;
                            temp.type = 0;
                            temp.vp = j;
                            DFS(temp, visited, ips, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, no_of_ips, ips_t_array);
                        }
                    }
                }

                for (int j = 0; j < size_vps_ingress; j++) { // for each Ingress
                    if (ingress[j][i] != 255) {
                        if (find_tree_seen(*visited, 1, j, i) == NULL) {
                            *ips = insert_tree(*ips, ips_t_array[i], -1);
                            *visited = insert_tree_seen(*visited, 1, j, i);
                            completeness_vp_struct temp;
                            temp.type = 1;
                            temp.vp = j;
                            DFS(temp, visited, ips, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, no_of_ips, ips_t_array);
                        }
                    }
                }

                for (int j = 0; j < size_vps_egress; j++) { // for each Egress
                    if (egress[j][i] != 255) {
                        if (find_tree_seen(*visited, 2, j, i) == NULL) {
                            *ips = insert_tree(*ips, ips_t_array[i], -1);
                            *visited = insert_tree_seen(*visited, 2, j, i);
                            completeness_vp_struct temp;
                            temp.type = 2;
                            temp.vp = j;
                            DFS(temp, visited, ips, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, no_of_ips, ips_t_array);
                        }
                    }
                }
            }
        }
    }
}

double get_subnet_completeness(int no_of_ips, int total_ips_in_subnet, short **ingress, short **egress, short **vps, int size_vps_ingress, int size_vps_egress, int size_vps_vp, int start, int end, uint32_t *ips_t_array, bool verbose) {
    // find the position of the largest VP
    completeness_vp_struct largest_vp_pos;
    largest_vp_pos.type = 0; // initialize
    largest_vp_pos.vp = 0; // initialize

    int max_ips_seen = 0;

    for (int i = 0; i < size_vps_vp; i++) { // for each VP
        int total = 0;

        for (int j = start; j <= end; j++) // for each IP
            if (vps[i][j] != 255)
                total++;

        if (total > max_ips_seen) {
            max_ips_seen = total;
            largest_vp_pos.type = 0;
            largest_vp_pos.vp = i;
        }

        if (max_ips_seen == no_of_ips)
            break;
    }

    for (int i = 0; i < size_vps_ingress; i++) { // for each Ingress
        int total = 0;

        for (int j = start; j <= end; j++) // for each IP
            if (ingress[i][j] != 255)
                total++;

        if (total > max_ips_seen) {
            max_ips_seen = total;
            largest_vp_pos.type = 1;
            largest_vp_pos.vp = i;
        }

        if (max_ips_seen == no_of_ips)
            break;
    }

    for (int i = 0; i < size_vps_egress; i++) { // for each Egress
        int total = 0;

        for (int j = start; j <= end; j++) // for each IP
            if (egress[i][j] != 255)
                total++;

        if (total > max_ips_seen) {
            max_ips_seen = total;
            largest_vp_pos.type = 2;
            largest_vp_pos.vp = i;
        }

        if (max_ips_seen == no_of_ips)
            break;
    }

    if (verbose)
        printf("\t\tLargest VP is at type %d vp %d, with ips %d\n", largest_vp_pos.type, largest_vp_pos.vp, max_ips_seen);

    // Get the IPs seen in the largest VP
    binarytree* ips = NULL; // contains IPs that are connected

    for (int i = start; i <= end; i++) { // for each IP
        if (largest_vp_pos.type == 0) {
            if (vps[largest_vp_pos.vp][i] != 255)
                ips = insert_tree(ips, ips_t_array[i], -1);
        }
        else if (largest_vp_pos.type == 1) {
            if (ingress[largest_vp_pos.vp][i] != 255)
                ips = insert_tree(ips, ips_t_array[i], -1);
        }
        else if (largest_vp_pos.type == 2) {
            if (egress[largest_vp_pos.vp][i] != 255)
                ips = insert_tree(ips, ips_t_array[i], -1);
        }
    }

    binarytree_seen* visited = NULL;

    DFS(largest_vp_pos, &visited, &ips, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, no_of_ips, ips_t_array);

    double numerator = size_tree(ips);
    double denominator = no_of_ips;

    if (verbose) {
        printf("---> %f\n", numerator);
        printf("---> %f\n", denominator);
    }

    return (numerator / denominator);
}

void check_giant_completeness(bool verbose, int separators_size, separators_struct* separators, uint32_t *ips_t_array, uint32_t *mask_one, int size_ips_t, double completeness_threshold, uint32_t *mask_zero, short **ingress, short **egress, short **vps, int size_vps_ingress, int size_vps_egress, int size_vps_vp) {
// with giant component
    int start = 0;
    int end = 0;

    while (start < separators_size) { // for each subnet
        if (separators[start].state) { // if true
            end = find_end(separators, separators_size, start);

            // get the mask
            int mask = check_special_ips(start, end, verbose, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, true);

            if (mask != -1) { // so that it would skip if split had occured
                bool split_occurs = false;

                if (mask < 30) {
                    // check completeness
                    int no_of_ips = (end - start) + 1;
                    int total_ips_in_subnet = pow(2, (32 - mask)) - 2;

                    double completeness_error = 1 - get_subnet_completeness(no_of_ips, total_ips_in_subnet, ingress, egress, vps, size_vps_ingress, size_vps_egress, size_vps_vp, start, end, ips_t_array, verbose);

                    if (verbose) {
                        printf("Block start: %d, end: %d\n", start, end);
                        printf("\tNo of ips between start and end: %d\n", no_of_ips);
                        printf("\tTotal no of ips in the subnet: %d\n\n", total_ips_in_subnet);
                        printf("\tComparing (completenes w/ threshold): %f - %f\n\n", completeness_error, completeness_threshold);
                    }

                    if (completeness_error > completeness_threshold) { // doesn't pass the threshold
                        int mask_to_match = mask+1;

                        if (verbose) {
                            printf("\tDoesn't pass the threshold\n");
                            printf("\tMask between start and end is: %d\n", mask);
                            printf("\tMask to match is: %d\n", mask_to_match);
                        }

                        // split
                        for (int i = start+1; i <= end; i++) { // for each ip other than 0 since that is what we are comparing with
                            //int mask_temp = get_mask_matches(start, i, mask_one, ips_t_array);
                            int mask_temp = check_special_ips(start, i, false, separators_size, separators, ips_t_array, mask_one, size_ips_t, mask_zero, false);
                            if (mask_temp < mask_to_match) {
                                split_occurs = true;
                                separators[i-1].state = false;

                                if (verbose)
                                    printf("\tChanging the state to FALSE at %d!\n", i-1);

                                break;
                            }
                        }
                    }
                    else if (verbose)
                        printf("\tPasses the threshold, skipping!\n");
                }

                if (!split_occurs)
                    start = end+1;
            }
            else if (verbose)
                printf("Splitting occured during special IP check, going back...\n");
        }
        else
            start++;

        if (verbose)
            printf("\n");
    }
}
*/
