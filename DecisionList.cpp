#include "DecisionList.h"

Decisions::DecisionData Decisions::DecisionData::Combine(const DecisionData& first, const DecisionData& second)
{
	DecisionData sum(std::min(first.turnsTillZeroWin, second.turnsTillZeroWin),
		std::min(first.turnsTillCrossWin, second.turnsTillCrossWin),
		std::min(first.turnsTillTie, second.turnsTillTie));
	sum.timesCrossWon = first.timesCrossWon + second.timesCrossWon;
	sum.timesZeroWon = first.timesZeroWon + second.timesZeroWon;
	sum.timesTie = first.timesTie + second.timesTie;
	return sum;
}