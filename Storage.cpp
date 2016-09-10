#include "Storage.hpp"
#include "Path.hpp"
#include <fstream>
using namespace std;

shared_ptr<Storage> Storage::m_instance = NULL;

Storage::Storage() {
	m_dirty = false;
	readFromFile();
}

bool Storage::readFromFile(void) {
	ifstream users_in, meetings_in;
	users_in.open(Path::userPath);
	if (!users_in) {
		return false;
	}
	string userString;
	while (getline(users_in, userString)) {
		int i, i1, i2, i3, i4;
		for (i = 1; i != userString.size(); i++) {
			if (userString[i] == '\"') {
				i1 = i;
				break;
			}
		}
		for (i = i1 + 3; i != userString.size(); i++) {
			if (userString[i] == '\"') {
				i2 = i;
				break;
			}
		}
		for (i = i2 + 3; i != userString.size(); i++) {
			if (userString[i] == '\"') {
				i3 = i;
				break;
			}
		}
		for (i = i3 + 3; i != userString.size(); i++) {
			if (userString[i] == '\"') {
				i4 = i;
				break;
			}
		}
		m_userList.push_back(User(userString.substr(1, i1 - 1),
			userString.substr(i1 + 3, i2 - i1 - 3),
			userString.substr(i2 + 3, i3 - i2 - 3),
			userString.substr(i3 + 3, i4 - i3 - 3)));
	}
	users_in.close();

	meetings_in.open(Path::meetingPath);
	if (!meetings_in) {
		return false;
	}
	string meetingString;
	while (getline(meetings_in, meetingString)) {
		int j, j1, j2, j3, j4, j5;
		for (j = 1; j != meetingString.size(); j++) {
			if (meetingString[j] == '\"') {
				j1 = j;
				break;
			}
		}
		for (j = j1 + 3; j != meetingString.size(); j++) {
			if (meetingString[j] == '\"') {
				j2 = j;
				break;
			}
		}
		for (j = j2 + 3; j != meetingString.size(); j++) {
			if (meetingString[j] == '\"') {
				j3 = j;
				break;
			}
		}
		for (j = j3 + 3; j != meetingString.size(); j++) {
			if (meetingString[j] == '\"') {
				j4 = j;
				break;
			}
		}
		for (j = j4 + 3; j != meetingString.size(); j++) {
			if (meetingString[j] == '\"') {
				j5 = j;
				break;
			}
		}
		string participator;
		participator = meetingString.substr(j1 + 3, j2 - j1 - 3);
		int b = 0;
		vector<string> participators;
		for (int a = 0; a != participator.size(); a++) {
			if (participator[a] == '&') {
				participators.push_back(participator.substr(b, a - b));
				b = a + 1;
			}
		}
		participators.push_back(participator.substr(b, participator.size() - b));
		m_meetingList.push_back(Meeting(meetingString.substr(1, j1 - 1),
			participators,
			Date::stringToDate(meetingString.substr(j2 + 3, j3 - j2 - 3)),
			Date::stringToDate(meetingString.substr(j3 + 3, j4 - j3 - 3)),
			meetingString.substr(j4 + 3, j5 - j4 - 3)));
	}
	meetings_in.close();
	return true;
}

bool Storage::writeToFile(void) {
	ofstream users_out, meetings_out;
	users_out.open(Path::userPath);
	if (!users_out) {
		return false;
	}
	for (list<User>::iterator i = m_userList.begin(); i != m_userList.end(); i++) {
		users_out << "\"" << i->getName() << "\",\"" << i->getPassword() << "\",\"" << i->getEmail() << "\",\"" << i->getPhone() << "\"" << endl;
	}
	users_out.close();

	meetings_out.open(Path::meetingPath);
	if (!meetings_out) {
		return false;
	}
	for (list<Meeting>::iterator i = m_meetingList.begin(); i != m_meetingList.end(); i++) {
		meetings_out << "\"" << i->getSponsor() << "\",\"" << i->getParticipator()[0];
		for (int j = 1; j != i->getParticipator().size(); j++) {
			meetings_out << '&' << i->getParticipator()[j];
		}
		
		meetings_out << "\",\"" << Date::dateToString(i->getStartDate()) << "\",\"" << Date::dateToString(i->getEndDate()) << "\",\"" << i->getTitle() << "\"" << endl;
	}
	meetings_out.close();
	return true;
}

shared_ptr<Storage> Storage::getInstance(void) {
	if (m_instance == NULL)  {
		m_instance = (shared_ptr<Storage>)(new Storage);
	}
	return m_instance;
}

Storage::~Storage() {
	sync();
	m_instance = NULL;
}


void Storage::createUser(const User & t_user) {
	m_userList.push_back(t_user);
	m_dirty = 1;
}

list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
	list<User> fitted;
	for (list<User>::const_iterator it = m_userList.begin(); it != m_userList.end(); ++it) {
		if (filter(*it)) {
			fitted.push_back(*it);
		}
	}
	return fitted;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher) {
	int num = 0;
	for (list<User>::iterator it = m_userList.begin(); it != m_userList.end(); it++) {
		if (filter(*it)) {
			switcher(*it);
			num++;
			m_dirty = 1;
		}
	}
	return num;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
	int num = 0;
	for (list<User>::iterator it = m_userList.begin(); it != m_userList.end(); ) {
		if (filter(*it)) {
			m_userList.erase(it++);
			num++;
			m_dirty = 1;
		} else it++;
	}
	return num;
}

void Storage::createMeeting(const Meeting & t_meeting) {
	m_meetingList.push_back(t_meeting);
	m_dirty = 1;
}

list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
	list<Meeting> fitted;
	for (list<Meeting>::const_iterator it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
		if (filter(*it)) {
			fitted.push_back(*it);
		}
	}
	return fitted;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher) {
	int num = 0;
	for (list<Meeting>::iterator it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
		if (filter(*it)) {
			switcher(*it);
			num++;
			m_dirty = 1;
		}
	}
	return num;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
	int num = 0;
	for (list<Meeting>::iterator it = m_meetingList.begin(); it != m_meetingList.end(); ) {
		if (m_meetingList.empty()) {
			break;
		}
		if (filter(*it)) {
			m_meetingList.erase(it++);
			num++;
			m_dirty = 1;
		} else it++;
	}
	return num;
}

bool Storage::sync(void) {
	if (writeToFile()) {
		m_dirty = 1;
		return true;
	} else {
		return false;
	}
}
