package com.infinite_imagination.util;

import java.util.Locale;

public class Dictionary {
	static String NATIVE_DICTIONARY_FILE_LOCATION = "dictionary.mp3";
    private static Dictionary sDictionary = null;
    private boolean mUsingNativeDictionary = false;
    private long mNativeDictionary = 0;
    
    public static synchronized Dictionary instance(Object androidAssetManager) {
    	if (sDictionary == null) sDictionary = new Dictionary(androidAssetManager);
    	return sDictionary;
    }
    
    private Dictionary(Object androidAssetManager) {
        mNativeDictionary = open(androidAssetManager, NATIVE_DICTIONARY_FILE_LOCATION);
    	if (mNativeDictionary != 0) {
    		mUsingNativeDictionary = true;
    		return;
    	}
    }
    
	public Boolean lookup(String wordToCheck) {
        String word = wordToCheck.toLowerCase(Locale.ENGLISH);
        
        if (mUsingNativeDictionary)
        	return find(mNativeDictionary, word);
        
       	return false;
	}
	
	private native long open(Object assetManager, String path);
	private native void close(long dictionary);
	private native boolean find(long dictionary, String word);
}

