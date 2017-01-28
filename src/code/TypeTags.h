//
// Created by withparadox2 on 2016/11/27.
//

#ifndef SIMPLECOMPILER_TYPETAGS_H
#define SIMPLECOMPILER_TYPETAGS_H

namespace TypeTags {
    enum {
        /** The tag of the basic type `byte'.
*/
        BYTE = 1,

        /** The tag of the basic type `char'.
         */
        CHAR = BYTE + 1,

        /** The tag of the basic type `short'.
         */
        SHORT = CHAR + 1,

        /** The tag of the basic type `int'.
         */
        INT = SHORT + 1,

        /** The tag of the basic type `long'.
         */
        LONG = INT + 1,

        /** The tag of the basic type `float'.
         */
        FLOAT = LONG + 1,

        /** The tag of the basic type `double'.
         */
        DOUBLE = FLOAT + 1,

        /** The tag of the basic type `boolean'.
         */
        BOOLEAN = DOUBLE + 1,

        /** The tag of the type `void'.
         */
        VOID = BOOLEAN + 1,

        /** The tag of all class and interface types.
         */
        CLASS = VOID + 1,

        /** The tag of all array types.
         */
        ARRAY = CLASS + 1,

        /** The tag of all (monomorphic) method types.
         */
        METHOD = ARRAY + 1,

        /** The tag of all package "types".
         */
        PACKAGE = METHOD + 1,

        /** The tag of all (source-level) type variables.
         */
        TYPEVAR = PACKAGE + 1,

        /** The tag of all type arguments.
         */
        WILDCARD = TYPEVAR + 1,

        /** The tag of all polymorphic (method-) types.
         */
        FORALL = WILDCARD + 1,

        /** The tag of the bottom type <null>.
         */
        BOT = FORALL + 1,

        /** The tag of a missing type.
         */
        NONE = BOT + 1,

        /** The tag of the error type.
         */
        ERROR = NONE + 1,

        /** The tag of an unknown type
         */
        UNKNOWN = ERROR + 1,

        /** The tag of all instantiatable type variables.
         */
        UNDETVAR = UNKNOWN + 1,

        /** The number of type tags.
         */
        TypeTagCount = UNDETVAR + 1,

        /** The maximum tag of a basic type.
         */
        lastBaseTag = BOOLEAN,

        /** The minimum tag of a partial type
         */
        firstPartialTag = ERROR
    };

};

#endif //SIMPLECOMPILER_TYPETAGS_H
