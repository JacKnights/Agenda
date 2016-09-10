#include "AgendaUI.hpp"
#include <iomanip>

using namespace std;

AgendaUI::AgendaUI() {
	startAgenda();
}


void AgendaUI::OperationLoop(void) {
	while (1) {
		cout << "--------------------Agenda--------------------" << endl;
		cout << "Action :                                      " << endl;
		cout << "l   - log in agenda by username and password  " << endl;
		cout << "r   - register an agenda account              " << endl;
		cout << "q   - quit agenda                             " << endl;
		cout << "--------------------Agenda--------------------" << endl << endl;
		cout << "Agenda ~$ ";
		if (!executeOperation(getOperation())) {
			break;
		}
	}
}

void AgendaUI::startAgenda(void) {
	m_userName = m_userPassword = "";
	m_agendaService.startAgenda();
}

    /**
     * catch user's operation
     * @return the operation
     */
std::string AgendaUI::getOperation() {
	string operation;
	cin >> operation;
	return operation;
}

    /**
     * execute the operation
     * @return if the operationloop continue
     */
bool AgendaUI::executeOperation(std::string operation) {
	if (operation == "l") {
		userLogIn();
		return 1;
	} else if (operation == "r") {
		userRegister();
		return 1;
	} else if (operation == "q") {
		quitAgenda();
		return 0;
	} else {
		return 1;
	}
	
}

    /**
     * user Login
     */
void AgendaUI::userLogIn(void) {
	cout << endl;
	cout << "[log in] [user name] [password]" << endl;
	cout << "[log in] ";
	string name, pass;
	cin >> name >> pass;
	if (m_agendaService.userLogIn(name, pass)) {
		m_userName = name;
		m_userPassword = pass;
		cout << "[log in] succeed!" << endl << endl;
		cout << "--------------------Agenda--------------------" << endl;
		cout << "Action :                                      " << endl;
		cout << "o   - log out Agenda                          " << endl;
		cout << "dc  - delete Agenda account                   " << endl;
		cout << "lu  - list all Agenda user                    " << endl;
		cout << "cm  - create a meeting                        " << endl;
		cout << "la  - list all meeting                        " << endl;
		cout << "las - list all sponsor meeting                " << endl;
		cout << "lap - list all participator meeting           " << endl;
		cout << "qm  - query meeting by title                  " << endl;
		cout << "qt  - query meeting by time interval          " << endl;
		cout << "dm  - delete meeting by title                 " << endl;
		cout << "da  - delete all meetings                     " << endl;
		cout << "----------------------------------------------" << endl << endl;
		cout << endl << "Agenda@" << name << " : # ";
		string operation;
		operation = getOperation();
		while (1) {
			if (operation == "o") {
				userLogOut();
				break;
			}
			if (operation == "dc") {
				deleteUser();
			}
			if (operation == "lu") {
				listAllUsers();
			}
			if (operation == "cm") {
				createMeeting();
			}
			if (operation == "la") {
				listAllMeetings();
			}
			if (operation == "las") {
				listAllSponsorMeetings();
			}
			if (operation == "lap") {
				listAllParticipateMeetings();
			}
			if (operation == "qm") {
				queryMeetingByTitle();
			}
			if (operation == "qt") {
				queryMeetingByTimeInterval();
			}
			if (operation == "dm") {
				deleteMeetingByTitle();
			}
			if (operation == "da") {
				deleteAllMeetings();
			}
			cout << "----------------------------------------------";
			cout  << endl << "Agenda@" << name << " : # ";
			operation = getOperation();
			cout << endl;
		}
	} else {
		cout << "[error] log in fail!" << endl;
	}
}

    /**
     * user regist
     */
void AgendaUI::userRegister(void) {
	string name, pass, email, phone;
	cout << "[register] [user name] [password] [email] [phone]" << endl;
	cout << "[register] ";
	cin >> name >> pass >> email >> phone;
	if (m_agendaService.userRegister(name, pass, email, phone)) {
		cout << "[register] succeed!" << endl;
	} else {
		cout << "[error] register fail!" << endl;
	}
}

    /**
     * quit the Agenda
     */
void AgendaUI::quitAgenda(void) {

}

    /**
     * user logout
     */
void AgendaUI::userLogOut(void) {
	
}

    /**
     * delete a user from storage
     */
void AgendaUI::deleteUser(void) {
	string name, pass;
	cout << "[delete Agenda account] [name] [password]" << endl;
	cout << "[delete Agenda account] ";
	cin >> name >> pass;
	if (m_agendaService.deleteUser(name, pass)) {
		cout << "[delete Agenda account] succeed!" << endl;
	} else {
		cout << "[error] delete Agenda account fail!" << endl;
	}
}

    /**
     * list all users from storage
     */
void AgendaUI::listAllUsers(void) {
	cout << "[list all users]" << endl << endl;
	cout << left << setw(8) << "name" <<left << setw(16) << "email" << left << "phone" << endl;
	list<User> U = m_agendaService.listAllUsers();
	for (list<User>::iterator it = U.begin(); it != U.end(); it++) {
		cout << left << setw(8) << it->getName() << left << setw(16) << it->getEmail() << left << it->getPhone() << endl;
	}
}

    /**
     * user create a meeting with someone else
     */
void AgendaUI::createMeeting(void) {
	cout << "[create meeting] [the number of participators]" << endl;
	cout << "[create meeting] ";
	int num;
	cin >> num;
	vector<string> pa;
	for (int i = 1; i <= num; i++) {
		cout << "[create meeting] [please enter participator " << i << " ]" << endl;
		cout << "[create meeting] " ;
		string name;
		cin >> name;
		pa.push_back(name);
	}
	string ti, st, en;
	cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cout << "[create meeting] " ;
	cin >> ti >> st >> en;
	if (m_agendaService.createMeeting(m_userName, ti, st, en, pa)) {
		cout << "[create meeting] succeed!"  << endl;
	} else {
		cout << "[error] create meeting fail!"  << endl;
	}
}

    /**
     * list all meetings from storage
     */
void AgendaUI::listAllMeetings(void) {
	cout << "[list all meetings]" << endl << endl;
	printMeetings(m_agendaService.listAllMeetings(m_userName));
}

    /**
     * list all meetings that this user sponsored
     */
void AgendaUI::listAllSponsorMeetings(void) {
	cout << "[list all sponsor meetings]" << endl << endl;
	printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
}

    /**
     * list all meetings that this user take part in
     */
void AgendaUI::listAllParticipateMeetings(void) {
	cout << "[list all participator meetings]" << endl << endl;
	printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
}

    /**
     * search meetings by title from storage
     */
void AgendaUI::queryMeetingByTitle(void) {
	cout << "[query meeting] [title]" << endl;
	cout << "[query meeting] ";
	string ti;
	cin >> ti;
	printMeetings(m_agendaService.meetingQuery(m_userName, ti));
}

    /**
     * search meetings by timeinterval from storage
     */
void AgendaUI::queryMeetingByTimeInterval(void) {
	cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
	cout << "[query meeting] ";
	string st, en;
	cin >> st >> en;
	printMeetings(m_agendaService.meetingQuery(m_userName, st, en));
}

    /**
     * delete meetings by title from storage
     */
void AgendaUI::deleteMeetingByTitle(void) {
	cout << "[delete meeting] [title]" << endl;
	cout << "[delete meeting] ";
	string ti;
	cin >> ti;
	if (m_agendaService.deleteMeeting(m_userName, ti)) {
		cout << "[delete meeting by title] succeed!" << endl;
	} else {
		cout << "[error] delete meeting by title fail!" << endl;
	}
}

    /**
     * delete all meetings that this user sponsored
     */
void AgendaUI::deleteAllMeetings(void) {
	if (m_agendaService.deleteAllMeetings(m_userName)) {
		cout << "[delete all meetings] succeed!" << endl;
	} else {
		cout << "[error] delete all meetings fail!" << endl;
	}
}

    /**
     * show the meetings in the screen
     */
void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
	cout << left << setw(16) << "title" << left << setw(16) << "sponsor" << left << setw(18) << "start time" << left << setw(18) << "end time" << left << "participator" << endl;
	list<Meeting> m = t_meetings;
	for (list<Meeting>::iterator it = m.begin(); it != m.end(); it++) {
		cout << left << setw(16) << it->getTitle() << left << setw(16) << it->getSponsor()
		<< left << setw(18)  << Date::dateToString(it->getStartDate())
		<< left<< setw(18) << Date::dateToString(it->getEndDate());
		vector<string> g = it->getParticipator();
		vector<string>::iterator v = g.begin();
		cout << *v;
		++v;
		for (; v != g.end(); v++) {
			cout << ',' << *v;
		}
		cout << endl;
	}
}
