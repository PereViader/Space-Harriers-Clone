#pragma once

class IDeletable {
public:
	IDeletable() : toDelete(false) {}

	bool ToDelete() const {
		return toDelete;
	}

	void MarkAsDeleted() {
		toDelete = true;
	}

private:
	bool toDelete;
};