#include "Parser.h"
using namespace std;

Parser::Parser(Bot &bot) : bot(bot) {
}

string Parser::NextCmd() {
	string k;
	getline(cmdLine, k, ' ');
	return k;
}

void Parser::Parse() {
	string line;
	while (getline(cin, line)){
		cmdLine.clear();
		cmdLine.str(line);
		// Process command
		string cmd = NextCmd();
		if      (cmd == "action")   ProcessAction();
		else if (cmd == "update")   ProcessUpdate();
		else if (cmd == "settings") ProcessSettings();
	}
}

void Parser::ProcessAction() {
	string cmd = NextCmd();
	if (cmd == "move") bot.Move(stoi(NextCmd()));
}

void Parser::ProcessUpdate() {
	NextCmd();
	string cmd = NextCmd();
	if      (cmd == "round") { bot.Round(stoi(NextCmd())); }
	else if (cmd == "field") { // Potentially replace with your own boardstate parser
	stringstream ss(NextCmd());
	Board board(ss);
	bot.Update(board);
	}
}

void Parser::ProcessSettings() {
	string cmd = NextCmd();
	if      (cmd == "timebank")      bot.Timebank(stoi(NextCmd()));
	else if (cmd == "time_per_move") bot.TimePerMove(stoi(NextCmd()));
	else if (cmd == "your_bot")      bot.YourBot(NextCmd());
	else if (cmd == "your_botid")   {
	int id = stoi(NextCmd());
	bot.YourBotId((id == 0) ? Pl1 : Pl2);
	}
	else if (cmd == "field_width")   { width  = stoi(NextCmd()); bot.FieldWidth(width);}
	else if (cmd == "field_height")  { height = stoi(NextCmd()); bot.FieldHeight(height);}
	else if (cmd == "player_names") {
	stringstream args(NextCmd());
	string player1, player2;
	getline(args, player1, ',');
	getline(args, player2, ',');
	bot.PlayerNames(player1,player1);}
}
