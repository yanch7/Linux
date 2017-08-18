#ifndef __NOCOPYABLE_H_
#define __NOCOPYABLE_H_

class nocopyable
{
protected:
	nocopyable() {}
	~nocopyable() {}
private:
	nocopyable(const nocopyable&);
	const nocopyable& operator= (const nocopyable&);
};

#endif