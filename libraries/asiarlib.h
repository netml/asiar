#include "binarytree_subnet.h"
#include "binarytree_ip.h"
#include "binarytree.h"
#include "binarytree_unresponsive.h"
#include "binarytree_duple.h"

#ifndef MY_HEADER_ASIARLIB
#define MY_HEADER_ASIARLIB

// struct which holds the separators
typedef struct separators_struct {
    bool state; // holds (true/false) stating that the 2 IPs are in the same subnet
} separators_struct;

// holds a trace
typedef struct trace {
    int start;
    int end;
    bool stays;
    bool first_ip_is_vp;
    bool last_ip_is_vp;
} trace;

typedef struct completeness_visited_struct {
    int type; // 0 for vp, 1 for ingress, 2 for egress
    int vp; // vp position
    int ip; // ip position
} completeness_visited_struct;

typedef struct completeness_vp_struct {
    int type; // 0 for vp, 1 for ingress, 2 for egress
    int vp; // vp position
} completeness_vp_struct;

void help();
void print_separators(int separators_size, separators_struct* separators, uint32_t *ips_t_array, uint32_t *mask_one, uint32_t *mask_zero, int size_ips_t);
void determine_separators(int mode, int separators_size, uint32_t *ips_t_array, separators_struct* separators, uint32_t *mask_one, uint32_t *mask_zero, int size_vps_vp, short **vps, int size_vps_ingress, short **ingress, int size_vps_egress, short **egress, int subnet_pair_error_threshold, binarytree_ip **conflict_set_subnet, int* conflict_set_subnet_positions, int size_ips_t);
int find_end(separators_struct* separators, int size, int start);
bool is_within_same_subnet(uint32_t data1, uint32_t data2, subnet* subnets_array, int subnets_size, uint32_t *mask_one);
bool ips_conflict_exists(binarytree_ip **conflict_set, uint32_t ip1, uint32_t ip2, int ip_pos_1, int ip_pos_2, int* conflict_set_positions);
binarytree* get_announcements(binarytree *announcements, char* AS, char const* announcement_filename);
bool mask_contains(mask* announcements_array, int announcements_size, uint32_t ip, uint32_t *mask_one, bool tmode);
void get_edges(trace* traces, binarytree_unresponsive** unresponsive, char const *file_path, binarytree_duple** aliases, uint32_t *ips_t_array, int size_ips_t, mask* announcements_array, int announcements_size, uint32_t *mask_one, bool verbose, bool tmode, subnet* subnets_array, int subnets_size, int *ips_subnet_id_array, int* reference_point_array, char const *duple_output_filename);
void get_ips_vps(trace* traces, binarytree_unresponsive** unresponsive, char const *file_path, binarytree_ip** vps_ingress, binarytree_ip** vps_egress, binarytree_ip** vps_vp, binarytree_ip** ips_t, binarytree_ip** ips_traced, mask* announcements_array, int announcements_size, uint32_t *mask_one, bool verbose, bool tmode, int no_of_traces, binarytree_ip* ground_truth_ips, char const* modified_traces_filename, bool write_traces, char const* midar_filename);
int check_special_ips(int start, int end, bool verbose, int separators_size, separators_struct* separators, uint32_t *ips_t_array, uint32_t *mask_one, int size_ips_t, uint32_t *mask_zero, bool split);
void fill_distances(trace* traces, binarytree_unresponsive** unresponsive, binarytree_ip **conflict_set_subnet, binarytree_ip **conflict_set, char const *file_path, uint32_t *ips_t_array, int size_ips_t, mask* announcements_array, int announcements_size, uint32_t *mask_one, uint32_t *vps_vp_array, int size_vps_vp, short **vps, uint32_t *vps_ingress_array, int size_vps_ingress, short **ingress, uint32_t *vps_egress_array, int size_vps_egress, short **egress, bool tmode, bool verbose, linkedlist **neighbors);
double check_if_alias_distance_matches(uint32_t IP1, uint32_t IP2, short **ingress, int size_vps_ingress, short **egress, int size_vps_egress, short **vps, int size_vps_vp, uint32_t *ips_t_array, int size_ips_t, int IP1_pos, int IP2_pos);
int* find_in_all(uint32_t *ips_t_array, int size_ips_t, binarytree_ip **conflict_set, int *conflict_set_positions, short **ingress, int size_vps_ingress, short **egress, int size_vps_egress, short **vps, int size_vps_vp, int start, int end, double subnet_group_error_threshold, int mode);
void compare_group_distances(binarytree_ip **conflict_set, int *conflict_set_positions, bool verbose, int separators_size, separators_struct* separators, short **ingress, short **egress, short **vps, int size_vps_ingress, int size_vps_egress, int size_vps_vp, uint32_t *ips_t_array, uint32_t *mask_one, uint32_t *mask_zero, int size_ips_t, double subnet_group_error_threshold, int mode);
void check_completeness(bool verbose, int separators_size, separators_struct* separators, uint32_t *ips_t_array, uint32_t *mask_one, int size_ips_t, double completeness_threshold, uint32_t *mask_zero);
binarytree_ip* get_ground_truth_ips(char const* ground_truth_ips_filename);
//void DFS(completeness_vp_struct largest_vp_pos, binarytree_seen **visited, binarytree **ips, short **ingress, short **egress, short **vps, int size_vps_ingress, int size_vps_egress, int size_vps_vp, int start, int end, int no_of_ips, uint32_t *ips_t_array);
//double get_subnet_completeness(int no_of_ips, int total_ips_in_subnet, short **ingress, short **egress, short **vps, int size_vps_ingress, int size_vps_egress, int size_vps_vp, int start, int end, uint32_t *ips_t_array, bool verbose);
//void check_giant_completeness(bool verbose, int separators_size, separators_struct* separators, uint32_t *ips_t_array, uint32_t *mask_one, int size_ips_t, double completeness_threshold, uint32_t *mask_zero, short **ingress, short **egress, short **vps, int size_vps_ingress, int size_vps_egress, int size_vps_vp);

#endif
