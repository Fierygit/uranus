//
// Created by Firefly on 2020/5/20.
//

#ifndef LAB3_SYNCDATAHANDLER_H
#define LAB3_SYNCDATAHANDLER_H

#include <vector>
#include "../common/public.h"
#include "../common/WaitGroup.h"

int getAliveCnt(Participants &participants);

void syncKVDB(Participants &participants);

void getLatestIndex(Participant *p, int idx, std::vector<int> &result);

std::vector<std::string> getLeaderData(Participant *p);

void syncOnePart(Participant *p, const std::vector<std::string> &leaderData, int maxIndex);

#endif //LAB3_SYNCDATAHANDLER_H
