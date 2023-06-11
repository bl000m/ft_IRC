#include "Server.hpp"

void	Server::mode(Client &client, Message const &mess)
{
	std::string	target;

	if (mess.getParamNum() < 1)
	{
		reply(client,  ERR_NEEDMOREPARAMS, "MODE", ":Not enough parameters");
		return ;
	}
	target = mess.getParam()[0];
	if (target[0] != '#' && target[0] != '&')
	{
		mode_user(client, mess, target);
	}
	else
	{
		mode_channel(client, mess, target);
	}
}

/*	helper	*/
void	Server::mode_user(Client &client, Message const &mess, std::string target)
{
	std::string	mode;

	if (!nick_in_use(target))
	{
		reply(client,  ERR_NOSUCHNICK, target.c_str(), ":No such nick");
		return ;
	}
	if (*client.getNick() != target)
	{
		reply(client,  ERR_USERSDONTMATCH, ":Cant change mode for other user", NULL);
		return ;
	}
	if (mess.getParamNum() < 2)
	{
		reply(client,  RPL_UMODEIS, client.getMode().c_str(), NULL);
		return ;
	}
	mode = mess.getParam()[1];
	if (client.setMode(mode))
		reply(client, ERR_UMODEUNKNOWNFLAG, ":Unknown MODE flag", NULL);
	reply(client, "MODE", client.getMode().c_str(), NULL);
}

/**
 CHANNEL MODES:
 - o : channel operator status to specified nickname
 - t : topic is locked and can only be changed by channel operator
 - k : key (password) protected channel => locks the channel with given password. When joining the channel every user must specify the password to access it
 - l : user limit => only the given number of members can be in the channel. If getUsersCount >= limit no other user is accepted
 - i : invite only => clients must be invited to join the channel when this mode is set
*/
void	Server::mode_channel(Client &client, Message const &mess, std::string target){
	std::string	mode;
	Channel *channel = this->getChannel(target);

	 if (channel == NULL) {
        this->reply(client,  ERR_NOSUCHCHANNEL, target.c_str(), ":No such channel");
        return;
    }

	if (mess.getParamNum() < 2){
		std::string channelModeMessage = ":FT_IRC 324 " + *(client.getNick()) + " "\
					 + target + " " + channel->getMode() + "\n";
		send(client.getSock(), channelModeMessage.c_str(), channelModeMessage.size(), 0);
		return ;
	}

	mode = mess.getParam()[1];
	if (!channel->isUserOperator(*(client.getNick()))) {
        this->reply(client,  ERR_CHANOPRIVSNEEDED, target.c_str(), ":You're not channel operator");
		return;
	}
}
