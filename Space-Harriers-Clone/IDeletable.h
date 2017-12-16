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

	void SetDeleted(bool state) {
		toDelete = state;
	}

private:
	bool toDelete;
};