#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <iostream>
# include <vector>

/*
	The default ctor creates an empty client

	The ctor Client(int sockfd, unsigned int ip) creates a connected client, which should be added to a container by the server

	If a client quit, the server should remove it from the 
	container, so the resources can be released
*/

/*
	The varaiable _pass and _regist:

	The client must first send PASS command, other commands will be ignored or regarded as unknown.The client can sent multiple PASS, only the value of last one will be used.

	The client can then send NICK, NIck can be set multiple time.

	The client can then set USER, once set cannot be modified.

	Once enter NICK command, the PASS will no longer be accepted.
	Once client fully registered, the_regist will be true. PASS USER will no longer be accepted
*/
class Client
{
	public:
		/*	canon form and ctor	*/
		Client(void);
		Client(Client const &client);
		~Client(void);
		Client	&operator=(Client const &client);

		Client(int sockfd, unsigned int ip);
		
		/*	setters	*/
		void	setPass(bool good);
		void	beRegist(void) const;
		void	setNick(std::string const &nick);
		void	setUser(std::string const &user);
		void	setHost(std::string const &host);

		/*	getters	*/
		int					getSock(void) const;
		unsigned int		getIp(void) const;
		bool				getPass(void) const;
		bool				isRegist(void) const;

		std::string const	*getNick(void) const;
		std::string const	*getUser(void) const;
		std::string const	*getHost(void) const;

	private:
		/*	private variable	*/
		int				_sock;
		unsigned int	_ip;
		bool			_pass;
		bool			_regist;
		std::string		*_nick;
		std::string		*_user;
		std::string		*_host;

		/*	private function	*/
		void	clear(void);
};

std::ostream	&operator<<(std::ostream &out, Client const &client);


#endif