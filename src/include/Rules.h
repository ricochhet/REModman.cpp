#ifndef __RE_RULESET_H
#pragma once

#include <iostream>
#include <vector>

struct Rule {
    bool is_directory;
    std::string name;
    std::string extension;
};

const std::vector<Rule> REENGINE_RULES = {
    {true, "reframework", ""},
    {true, "natives", ""},
    {false, "", ".pak"}
};


#endif __RE_RULESET_H