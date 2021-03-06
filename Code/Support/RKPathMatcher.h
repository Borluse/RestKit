//
//  RKPathMatcher.h
//  RestKit
//
//  Created by Greg Combs on 9/2/11.
//  Copyright (c) 2011 RestKit. All rights reserved.
//

#import <Foundation/Foundation.h>
@class SOCPattern;

/**
 This class performs pattern matching and parameter parsing of strings, usually resource paths.
 It provides much of the necessary tools to map a given resource path to local objects (the inverse 
 of RKRouter's function).  This makes it easier to implement RKManagedObjectCache, and generate fetched
 requests from a given resource path.  There are two means of instantiating and using a matcher object
 in order to provide more flexibility in implementations, and to improve efficiency by eliminating
 repetitive and costly pattern initializations.
   
 @see RKManagedObjectCache
 @see RKMakePathWithObject
 @see RKRouter
 */
@interface RKPathMatcher : NSObject {
@private
    SOCPattern *socPattern_;
    NSString *sourcePath_;
    NSString *rootPath_;
    NSDictionary *queryParameters_;
}
@property (retain,readonly) NSDictionary *queryParameters;

/**
 Creates an RKPathMatcher starting from a resource path string.  This method should be followed by 
 matchesPattern:tokenizeQueryStrings:parsedArguments:
 
 @param pathString The string to evaluate and parse, such as /districts/tx/upper/?apikey=GC5512354
 @return An instantiated RKPathMatcher without an established pattern.
 */
+(RKPathMatcher *)matcherWithPath:(NSString *)pathString;

/**
 Determines if the path string matches the provided pattern, and yields a dictionary with the resulting 
 matched key/value pairs.  Use of this method should be preceded by matcherWithPath:
 Pattern strings should include encoded parameter keys, delimited by a single colon at the 
 beginning of the key name.  
 
 *NOTE* - Numerous colon-encoded parameter keys can be joined in a long pattern, but each key must be 
 separated by at least one unmapped character.  For instance, /:key1:key2:key3/ is invalid, wheras
 /:key1/:key2/:key3/ is acceptable.
 
 @param patternString The pattern to use for evaluating, such as /:entityName/:stateID/:chamber/
 @param shouldTokenize If YES, any query parameters will be tokenized and inserted into the parsed argument dictionary.
 @param arguments A pointer to a dictionary that contains the key/values from the pattern (and parameter) matching.
 @return A boolean indicating if the path string successfully matched the pattern. 
 */
- (BOOL)matchesPattern:(NSString *)patternString tokenizeQueryStrings:(BOOL)shouldTokenize parsedArguments:(NSDictionary **)arguments;

/**
 Creates an RKPathMatcher starting from a pattern string.  This method should be followed by 
 matchesPath:tokenizeQueryStrings:parsedArguments:  Patterns should include encoded parameter keys,
 delimited by a single colon at the beginning of the key name.  
 
 *NOTE* - Numerous colon-encoded parameter keys can be joined in a long pattern, but each key must be 
 separated by at least one unmapped character.  For instance, /:key1:key2:key3/ is invalid, wheras
 /:key1/:key2/:key3/ is acceptable.
 
 @param patternString The pattern to use for evaluating, such as /:entityName/:stateID/:chamber/
 @return An instantiated RKPathMatcher with an established pattern. 
 */
+(RKPathMatcher *)matcherWithPattern:(NSString *)patternString;

/**
 Determines if the provided resource path string matches a pattern, and yields a dictionary with the resulting 
 matched key/value pairs.  Use of this method should be preceded by matcherWithPattern:
 
 @param pathString The string to evaluate and parse, such as /districts/tx/upper/?apikey=GC5512354
 @param shouldTokenize If YES, any query parameters will be tokenized and inserted into the parsed argument dictionary.
 @param arguments A pointer to a dictionary that contains the key/values from the pattern (and parameter) matching.
 @return A boolean indicating if the path string successfully matched the pattern. 
 */
- (BOOL)matchesPath:(NSString *)pathString tokenizeQueryStrings:(BOOL)shouldTokenize parsedArguments:(NSDictionary **)arguments;

/**
 This generates a resource path by interpolating the properties of the 'object' argument, assuming the existence
 of a previously specified pattern established via matcherWithPattern:.  Otherwise, this method is identical in
 function to RKMakePathWithObject (in fact it is a shortcut for this method).
  
 For example, given an 'article' object with an 'articleID' property value of 12345 ...
 
   RKPathMatcher *matcher = [RKPathMatcher matcherWithPattern:@"/articles/:articleID"];
   NSString *resourcePath = [matcher pathFromObject:article];
 
 ... will produce a 'resourcePath' containing the string "/articles/12345"
    
 @param object The object containing the properties to interpolate.
 @return A string with the object's interpolated property values inserted into the receiver's established pattern.
 @see RKMakePathWithObject
 @see RKRouter
 */
- (NSString *)pathFromObject:(id)object;

@end
