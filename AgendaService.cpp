#include "AgendaService.hpp"
using namespace std;

AgendaService::AgendaService() {
	startAgenda();
}

AgendaService::~AgendaService() {
	quitAgenda();
}

bool AgendaService::userLogIn(const std::string userName, const std::string password) {
	list<User> u = listAllUsers();
	if (u.empty()) {
		return false;
	}
	for (list<User>::iterator it = u.begin(); it != u.end(); it++) {
		if (it->getName() == userName) {
			return it->getPassword() == password;
		}
	}
	return false;
}

bool AgendaService::userRegister(const std::string userName, const std::string password,
                      const std::string email, const std::string phone) {
	list<User> u = m_storage->queryUser([userName](const User &U)->bool{
		return (U.getName() == userName);
	});
	if (u.empty()) {
		m_storage->createUser(User(userName, password, email, phone));
		return true;
	}
	return false;
}

bool AgendaService::deleteUser(const std::string userName, const std::string password) {
	int r = m_storage->deleteUser([userName, password](const User & Us)->bool{
		return (Us.getName() == userName && Us.getPassword() == password);
	});
	if (r != 0) {
		deleteAllMeetings(userName);
		return true;
	} else
		return false;
}

std::list<User> AgendaService::listAllUsers(void) const {
	return m_storage->queryUser([](const User &U)->bool{return 1;});
}

bool AgendaService::createMeeting(const std::string userName, const std::string title,
                       const std::string startDate, const std::string endDate,
                       const std::vector<std::string> participator) {
	Date start = Date::stringToDate(startDate), end = Date::stringToDate(endDate);
list<User> users = listAllUsers();
	list<User>::iterator j;
	for (j = users.begin(); j != users.end(); j++) {
		if (userName == j->getName()) break;
	}
	if (j == users.end()) return false;
	if (start >= end) return false;
	if (participator.empty()) return false;
	if (!(Date::isValid(start) && Date::isValid(end))) return false;

	
	
	for (vector<string>::const_iterator it = participator.begin(); it != participator.end(); it++) {
		if (*it == userName) return false;
		list<User>::iterator i;
		for (i = users.begin(); i != users.end(); i++) {
			if (*it == i->getName()) break;
		}
		if (i == users.end()) return false;
	}

	
	list<Meeting> meetings = m_storage->queryMeeting([title](const Meeting& M)->bool {
		if (title == M.getTitle()) {
			return true;
		}
		return false;
	});
	if (!meetings.empty()) return false;
for (vector<string>::const_iterator it = participator.begin(); it != participator.end(); it++) {
		string tmp = *it;
		meetings = m_storage->queryMeeting([tmp](const Meeting& M)->bool {
			vector<string> part = M.getParticipator();
			vector<string>::iterator iter;
			for (iter = part.begin(); iter != part.end(); iter++) {
				if (tmp == *iter) return true;
			}
			return (tmp == M.getSponsor());
		});
		for (list<Meeting>::iterator i = meetings.begin(); i != meetings.end(); i++) {
			if ((end > i->getStartDate() && end < i->getEndDate()) ||
			 (start > i->getStartDate() && start < i->getEndDate()) ||
			 (start <= i->getStartDate() && end >= i->getEndDate())) {
				return false;
			}
		}
	}
	meetings = m_storage->queryMeeting([userName](const Meeting& M)->bool {
		vector<string> tm = M.getParticipator();
		vector<string>::iterator is;
		for (is = tm.begin(); is != tm.end(); is++) {
			if (userName == *is) return true;
		}
		return (userName == M.getSponsor());
	});
	for (list<Meeting>::iterator i = meetings.begin(); i != meetings.end(); i++) {
		if ((end > i->getStartDate() && end < i->getEndDate()) || 
			(start > i->getStartDate() && start < i->getEndDate()) ||
			(start <= i->getStartDate() && end >= i->getEndDate())) {
			return false;
		}
	}
	




	for (int g = 0; g != participator.size(); g++) {
		for (int h = 0; h != participator.size();h++) {
			if (participator[g] == participator[h] && g != h) return false;
		}
	}
	m_storage->createMeeting(Meeting(userName, participator, start, end, title));
	return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string title) const {
	return m_storage->queryMeeting([userName, title](const Meeting& M)->bool {
		if (M.getTitle() == title) {
			if ((userName == M.getSponsor()) || M.isParticipator(userName)) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	});
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string startDate,
                                    const std::string endDate) const {
	list<Meeting> empty_list;
	Date sd(startDate);
	Date ed(endDate);
	if (!(Date::isValid(sd) && Date::isValid(ed))) return empty_list;
	return m_storage->queryMeeting([userName, sd, ed](const Meeting& M)->bool {
		if (M.getStartDate() <= ed && M.getEndDate() >= sd) {
			if ((userName == M.getSponsor()) || M.isParticipator(userName)) {
				return true;
			} else {
				return false;
			}
			
		} else {
			return false;
		}
	});
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string userName) const {
	return m_storage->queryMeeting([&](const Meeting& M)->bool{
		if (userName == M.getSponsor() || M.isParticipator(userName)) {
			return 1;
		} else {
			return 0;
		}
		});
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string userName) const {
	return m_storage->queryMeeting([&](const Meeting& M)->bool{
		if (userName == M.getSponsor()) {
			return 0;
		} else {
			return 1;
		}
		});
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string userName) const {
	return m_storage->queryMeeting([&](const Meeting& M)->bool{
		if (M.isParticipator(userName)) {
			return 1;
		} else {
			return 0;
		}
		});
}

bool AgendaService::deleteMeeting(const std::string userName, const std::string title) {
	return m_storage->deleteMeeting([userName, title](const Meeting& M)->bool{
		return (userName == M.getSponsor() && title == M.getTitle());
		});
}

bool AgendaService::deleteAllMeetings(const std::string userName) {
	return m_storage->deleteMeeting([userName](const Meeting& M)->bool{
		return (userName == M.getSponsor());
		});
}

void AgendaService::startAgenda(void) {
	m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
	m_storage->sync();
}
