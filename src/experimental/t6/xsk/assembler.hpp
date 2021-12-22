// Copyright 2021 xensik. All rights reserved.
//
// Use of this source code is governed by a GNU GPLv3 license
// that can be found in the LICENSE file.

#pragma once

namespace xsk::gsc::t6
{

enum import_type
{
    ptr_reference = 1,
    function_call = 2,
    thread_call = 3,
    method_call = 4,
    thread_method_call = 5,
};

struct function_import
{
    std::string function_name;
    std::vector<int> references;
    size_t num_args;
    import_type type;
    bool called_from_devblock;
};

class assembler : public gsc::assembler
{
    std::string m_filename;
    utils::byte_buffer_ptr m_bytecode;
    std::unordered_map<std::uint32_t, std::string> m_labels;
    std::vector<gsc::function_ptr> m_functions;
    
    // gathered on the assemble pass through the instructions, needed for file metadata
    std::unordered_map<std::string, std::vector<int>> m_strings;
    // todo: animtrees
    std::unordered_map<std::string, function_import> m_imports;

public:
    auto output_script() -> std::vector<std::uint8_t>;
    auto output_stack() -> std::vector<std::uint8_t>;
    void assemble(const std::string& file, std::vector<std::uint8_t>& data);
    void assemble(const std::string& file, std::vector<gsc::function_ptr>& functions);

private:
    void assemble_function(const gsc::function_ptr& func);
    void assemble_instruction(const gsc::instruction_ptr& inst);
    void assemble_builtin_call(const gsc::instruction_ptr& inst, bool method, bool arg_num);
    void assemble_local_call(const gsc::instruction_ptr& inst, bool thread);
    void assemble_far_call(const gsc::instruction_ptr& inst, bool thread);
    void assemble_switch(const gsc::instruction_ptr& inst);
    void assemble_end_switch(const gsc::instruction_ptr& inst);
    void assemble_field_variable(const gsc::instruction_ptr& inst);
    void assemble_jump(const gsc::instruction_ptr& inst, bool expr, bool back);
    void assemble_offset(std::int32_t offset);
    auto resolve_function(const std::string& name) -> std::uint32_t;
    auto resolve_label(const std::string& name) -> std::uint32_t;
};

} // namespace xsk::gsc::t6
