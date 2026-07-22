#ifndef LOONG_EMU_STUDIO_PROTOCOL_H
#define LOONG_EMU_STUDIO_PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

typedef struct CPUArchState CPULoongArchState;

void studio_protocol_set_enabled(bool enabled);
bool studio_protocol_enabled(void);
bool studio_protocol_hit_breakpoint(CPULoongArchState *env);
bool studio_protocol_reached_instruction_limit(CPULoongArchState *env);
void studio_protocol_request_stop(void);
bool studio_protocol_stop_requested(void);
void studio_protocol_record_memory_access(CPULoongArchState *env,
        const char *op, uint64_t address, uint64_t physical_address,
        int size, uint64_t value);
void studio_protocol_dump_final_state(CPULoongArchState *env);

#endif
