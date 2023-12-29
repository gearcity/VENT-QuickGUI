#include "QuickGUIScriptReader.h"
#include "QuickGUIScriptDefinition.h"

QuickGUI::ScriptReader* QuickGUI::ScriptReader::mSingletonPtr = NULL;

namespace QuickGUI
{
	_QuickGUIExport void registerScriptReader()
	{
		OGRE_NEW ScriptReader();
	}

	ScriptReader::ScriptReader() :
		mBegin(false)
	{
		//Init singleton
		if (mSingletonPtr != NULL)
			OGRE_EXCEPT(Ogre::Exception::ExceptionCodes::ERR_INVALIDPARAMS, "Multiple ScriptParser objects are not allowed", "ScriptParser::ScriptParser");
		mSingletonPtr = this;

		//Register as a ScriptLoader
		mLoadOrder = 100.0f;

		mScriptPatterns.push_back("*.skinTypes");

		// Register as Script Loader
		Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

	}

	ScriptReader::~ScriptReader()
	{
	for(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >::iterator it1 = mDefinitions.begin(); it1 != mDefinitions.end(); ++it1)
		{

			for(std::map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)	
			{
				OGRE_DELETE_T((*it2).second,ScriptDefinition,Ogre::MEMCATEGORY_GENERAL);
			}
		}

		for(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >::iterator it1 = mTempDefinitions.begin(); it1 != mTempDefinitions.end(); ++it1)	
		{

			for(std::map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)	
			{
				OGRE_DELETE_T((*it2).second,ScriptDefinition,Ogre::MEMCATEGORY_GENERAL);
			}
		}
	}

	ScriptReader* ScriptReader::getSingletonPtr(void) 
	{ 
		return mSingletonPtr; 
	}

	ScriptReader& ScriptReader::getSingleton(void) 
	{ 
		assert( mSingletonPtr );  
		return ( *mSingletonPtr ); 
	}

	float ScriptReader::getLoadingOrder() const
	{
		return mLoadOrder;
	}

	const Ogre::StringVector& ScriptReader::getScriptPatterns() const
	{
		return mScriptPatterns;
	}

	void ScriptReader::parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
	{
		// Read in all data
		while(!stream->eof())
		{
			_convertToTokens(stream->getLine(),mTokens);
		}
		mTokens.push_back(Token(QuickGUI::Token::TYPE_EOF,""));

		_createDefinitions(mTokens,mDefinitions);

		// We no longer need the tokens, get rid of them. (There are probably quite a few!)
		mTokens.clear();
	}

	void ScriptReader::_convertToTokens(Ogre::String s, std::vector<Token>& tokenList)
	{
		int index = 0;
		while(index < static_cast<int>(s.length()))
		{
			if(s[index] == '{')
			{
				// merge the last two TYPE_TEXT tokens into a "definition" token
				int lastTokenIndex = static_cast<int>(tokenList.size() - 1);
				if((lastTokenIndex < 2) || (tokenList[lastTokenIndex].type != QuickGUI::Token::TYPE_NEWLINE) || (tokenList[lastTokenIndex].type != QuickGUI::Token::TYPE_NEWLINE))
					throw Exception(Exception::ERR_SCRIPT_PARSING,"Invalid syntax for Definition!","ScriptReader::_convertToTokens");

				Ogre::String type = tokenList[lastTokenIndex - 2].value;
				Ogre::String id = tokenList[lastTokenIndex - 1].value;
				tokenList.pop_back(); // remove newline
				tokenList.pop_back(); // remove text
				tokenList.pop_back(); // remove text
				tokenList.push_back(Token(QuickGUI::Token::TYPE_DEFINITION,type + " " + id)); // add definition
				tokenList.push_back(Token(QuickGUI::Token::TYPE_NEWLINE,"\n")); // add newline
				tokenList.push_back(Token(QuickGUI::Token::TYPE_OPENBRACE,"{"));

				++index;
			}
			else if(s[index] == '}')
			{
				tokenList.push_back(Token(QuickGUI::Token::TYPE_CLOSEBRACE,"}"));
				++index;
			}
			else if(!isspace(s[index]))
			{
				Ogre::String text = "";
				while((index < static_cast<int>(s.length())) && !isspace(s[index]) && ((s[index] != '\n') || (s[index] != '\r')))
				{
					text += s[index];
					++index;
				}

				// Determine if this portion of text is a property name or a property value.
				// A property name will have a newline in front of it.
				int lastTokenIndex = static_cast<int>(tokenList.size() - 1);
				if((lastTokenIndex > 0) && (tokenList[lastTokenIndex].type == QuickGUI::Token::TYPE_NEWLINE))
					tokenList.push_back(Token(QuickGUI::Token::TYPE_PROPERTY,text));
				else
					tokenList.push_back(Token(QuickGUI::Token::TYPE_TEXT,text));
			}
			else
			{
				++index;
			}
		}

		tokenList.push_back(Token(QuickGUI::Token::TYPE_NEWLINE,"\n"));
	}


	void ScriptReader::_createDefinitions(std::vector<Token>& tokenList, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList)
	{
		int tokenIndex = 0;

		ScriptDefinition* currentDefinition = NULL;

		Token* currentToken = &(tokenList[tokenIndex]);

		while(1)
		{
			switch(currentToken->type)
			{
			case QuickGUI::Token::TYPE_DEFINITION:
				{
					int index = static_cast<int>(currentToken->value.find_first_of(' '));
					Ogre::String type = currentToken->value.substr(0,index);
					Ogre::String id = currentToken->value.substr(index+1);
					ScriptDefinition* newDefinition = OGRE_NEW_T(ScriptDefinition,Ogre::MEMCATEGORY_GENERAL)(type,id);

					newDefinition->mParentDefinition = currentDefinition;

					if(currentDefinition == NULL)
					{
						if(defList[type].find(id) != defList[type].end())
							throw Exception(Exception::ERR_SCRIPT_PARSING,"Definition of type \"" + type + "\" and name \"" + id + "\" already exists, overwriting would result in loss of data!","ScriptReader::_createDefinitions");
						defList[type][id] = newDefinition;
					}
					else
					{
						if(currentDefinition->mSubDefinitions[type].find(id) != currentDefinition->mSubDefinitions[type].end())
							throw Exception(Exception::ERR_SCRIPT_PARSING,"Definition of type \"" + type + "\" and name \"" + id + "\" already exists, overwriting would result in loss of data!","ScriptReader::_createDefinitions");
						currentDefinition->mSubDefinitions[type][id] = newDefinition;
						currentDefinition->mSubDefinitionsInOrder.push_back(newDefinition);
					}

					currentDefinition = newDefinition;
				}
				break;
			case QuickGUI::Token::TYPE_PROPERTY:
				{
					Ogre::String propertyName = currentToken->value;
					if(currentDefinition->mProperties.find(propertyName) != currentDefinition->mProperties.end())
						throw Exception(Exception::ERR_SCRIPT_PARSING,"Property with name \"" + propertyName + "\" already exists, overwriting would result in loss of data!","ScriptReader::_createDefinitions");
					DefinitionProperty* newProperty = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);

					// Advance to next Token
					++tokenIndex;
					currentToken = &(tokenList[tokenIndex]);
					while(currentToken->type == QuickGUI::Token::TYPE_TEXT)
					{
						newProperty->mValues.push_back(currentToken->value);

						++tokenIndex;
						currentToken = &(tokenList[tokenIndex]);
					}

					--tokenIndex;
					currentToken = &(tokenList[tokenIndex]);

					currentDefinition->mProperties[propertyName] = newProperty;
				}
				break;
			case QuickGUI::Token::TYPE_CLOSEBRACE:
				currentDefinition = currentDefinition->mParentDefinition;
				break;
			case QuickGUI::Token::TYPE_EOF:
				return;
			}

			// Advance to next Token
			++tokenIndex;
			currentToken = &(tokenList[tokenIndex]);
		}
	}

#if USEHASHMAPS
	ScriptDefinition* ScriptReader::_findDefinition(const Ogre::String& type, const Ogre::String& id, stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >& defList)
#else
	ScriptDefinition* ScriptReader::_findDefinition(const Ogre::String& type, const Ogre::String& id, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList)
#endif	
	{
		ScriptDefinition* d = NULL;
#if USEHASHMAPS
	for(stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >::iterator it1 = defList.begin(); it1 != defList.end(); ++it1)
#else
	for(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >::iterator it1 = defList.begin(); it1 != defList.end(); ++it1)
#endif	
		{
#if USEHASHMAPS
			for(stdext::hash_map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
#else
			for(std::map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
#endif				
			{
				d = (*it2).second->findDefinition(type,id);
				if(d != NULL)
					return d;
			}
		}

		return NULL;
	}
#if USEHASHMAPS
	ScriptDefinition* ScriptReader::_getDefinition(const Ogre::String& type, const Ogre::String& id, stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >& defList)
#else
	ScriptDefinition* ScriptReader::_getDefinition(const Ogre::String& type, const Ogre::String& id, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList)
#endif
	{
		if(defList.find(type) == defList.end())
			throw Exception(Exception::ERR_SCRIPT_PARSING,"No definition for type \"" + type + "\" found!","ScriptReader::_getDefinition");

		if(defList[type].find(id) == defList[type].end())
			throw Exception(Exception::ERR_SCRIPT_PARSING,"No \"" + type + "\" definition with id \"" + id + "\" found!","ScriptReader::_getDefinition");

		return defList[type][id];
	}

#if USEHASHMAPS
	std::list<ScriptDefinition*> ScriptReader::_getDefinitions(stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >& defList)
#else
	std::list<ScriptDefinition*> ScriptReader::_getDefinitions(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList)
#endif
	{
		std::list<ScriptDefinition*> dList;
#if USEHASHMAPS
	for(stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >::iterator it1 = defList.begin(); it1 != defList.end(); ++it1)
#else
	for(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >::iterator it1 = defList.begin(); it1 != defList.end(); ++it1)
#endif
		{
#if USEHASHMAPS
	for(stdext::hash_map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
#else
	for(std::map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
#endif			
			{
				dList.push_back((*it2).second);
			}
		}

		return dList;
	}

#if USEHASHMAPS
	std::list<ScriptDefinition*> ScriptReader::_getDefinitions(const Ogre::String& type, stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >& defList)
#else
	std::list<ScriptDefinition*> ScriptReader::_getDefinitions(const Ogre::String& type, std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >& defList)
#endif	
	{
		if(defList.find(type) == defList.end())
			throw Exception(Exception::ERR_SCRIPT_PARSING,"No definitions for type \"" + type + "\" found!","ScriptReader::_getDefinitions");

		std::list<ScriptDefinition*> dList;
#if USEHASHMAPS
	for(stdext::hash_map<Ogre::String,ScriptDefinition*>::iterator it = defList[type].begin(); it != defList[type].end(); ++it)
#else
	for(std::map<Ogre::String,ScriptDefinition*>::iterator it = defList[type].begin(); it != defList[type].end(); ++it)
#endif			
			dList.push_back((*it).second);

		return dList;
	}

	void ScriptReader::begin()
	{
		if(mBegin)
			throw Exception(Exception::ERR_SCRIPT_PARSING,"Already doing isolated parsing!  Did you forget to call end()?","ScriptReader::begin");

		mBegin = true;
	}

	void ScriptReader::end()
	{
		if(!mBegin)
			throw Exception(Exception::ERR_SCRIPT_PARSING,"Isolated parsing not started!  Did you forget to call begin()?","ScriptReader::end");

		mBegin = false;
		
#if USEHASHMAPS
		for(stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >::iterator it1 = mTempDefinitions.begin(); it1 != mTempDefinitions.end(); ++it1)
#else
		for(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >::iterator it1 = mTempDefinitions.begin(); it1 != mTempDefinitions.end(); ++it1)
#endif			
		{
#if USEHASHMAPS
			for(stdext::hash_map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
#else
			for(std::map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
#endif			
			{
				OGRE_DELETE_T((*it2).second,ScriptDefinition,Ogre::MEMCATEGORY_GENERAL);
			}
		}
		mTempDefinitions.clear();
	}

	ScriptDefinition* ScriptReader::findDefinition(const Ogre::String& type, const Ogre::String& id)
	{
		if(mBegin)
			return _findDefinition(type,id,mTempDefinitions);
		else
			return _findDefinition(type,id,mDefinitions);
	}

	std::list<ScriptDefinition*> ScriptReader::getDefinitions()
	{
		if(mBegin)
			return _getDefinitions(mTempDefinitions);
		else
			return _getDefinitions(mDefinitions);
	}

	std::list<ScriptDefinition*> ScriptReader::getDefinitions(const Ogre::String& type)
	{
		if(mBegin)
			return _getDefinitions(type,mTempDefinitions);
		else
			return _getDefinitions(type,mDefinitions);
	}

	ScriptDefinition* ScriptReader::getDefinition(const Ogre::String& type, const Ogre::String& id)
	{
		if(mBegin)
			return _getDefinition(type,id,mTempDefinitions);
		else
			return _getDefinition(type,id,mDefinitions);
	}

	std::list<ScriptDefinition*> ScriptReader::parseFile(const Ogre::String& fileName)
	{
		std::ifstream stream(fileName.c_str());

		std::vector<Token> tokens;

		// Read in all data
		char str[256];
		while(stream.getline(str,256))
		{
			_convertToTokens(str,tokens);
		}
		tokens.push_back(Token(QuickGUI::Token::TYPE_EOF,""));

#if USEHASHMAPS
		stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> > defs;
#else
		std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> > defs;
#endif
		
		if(mBegin)
			_createDefinitions(tokens,mTempDefinitions);
		else
			_createDefinitions(tokens,mDefinitions);

		// We no longer need the tokens, get rid of them. (There are probably quite a few!)
		tokens.clear();

		std::list<ScriptDefinition*> defList;
#if USEHASHMAPS
		for(stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >::iterator it1 = defs.begin(); it1 != defs.end(); ++it1)		
#else
		for(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >::iterator it1 = defs.begin(); it1 != defs.end(); ++it1)		
#endif
		{
#if USEHASHMAPS
			for(stdext::hash_map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
#else
			for(std::map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
#endif			
			{
				defList.push_back((*it2).second);
			}
		}

		return defList;
	}

	std::list<ScriptDefinition*> ScriptReader::parseString(const Ogre::String& s)
	{
		std::vector<Token> tokens;

		_convertToTokens(s,tokens);
		mTokens.push_back(Token(QuickGUI::Token::TYPE_EOF,""));

#if USEHASHMAPS
		stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> > defs;
#else
		std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> > defs;
#endif
		
		if(mBegin)
			_createDefinitions(tokens,mTempDefinitions);
		else
			_createDefinitions(tokens,mDefinitions);

		// We no longer need the tokens, get rid of them. (There are probably quite a few!)
		tokens.clear();

		std::list<ScriptDefinition*> defList;

#if USEHASHMAPS
		for(stdext::hash_map<Ogre::String, stdext::hash_map<Ogre::String,ScriptDefinition*> >::iterator it1 = defs.begin(); it1 != defs.end(); ++it1)
#else
		for(std::map<Ogre::String, std::map<Ogre::String,ScriptDefinition*> >::iterator it1 = defs.begin(); it1 != defs.end(); ++it1)
#endif
		{
#if USEHASHMAPS
		for(stdext::hash_map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
#else
		for(std::map<Ogre::String,ScriptDefinition*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
#endif			
			{
				defList.push_back((*it2).second);
			}
		}

		return defList;
	}
}
