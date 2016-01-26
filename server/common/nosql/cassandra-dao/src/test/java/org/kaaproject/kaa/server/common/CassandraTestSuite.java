/*
 * Copyright 2014-2016 CyberVision, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.kaaproject.kaa.server.common;

import org.cassandraunit.CassandraCQLUnit;
import org.cassandraunit.dataset.cql.ClassPathCQLDataSet;
import org.junit.ClassRule;
import org.junit.extensions.cpsuite.ClasspathSuite;
import org.junit.extensions.cpsuite.ClasspathSuite.ClassnameFilters;
import org.junit.runner.RunWith;

@RunWith(ClasspathSuite.class)
@ClassnameFilters({"org.kaaproject.kaa.server.common.dao.service.Cassandra.*Test", 
                   "org.kaaproject.kaa.server.common.nosql.cassandra.dao.*Test"})
public class CassandraTestSuite {

    @ClassRule
    public static CassandraCQLUnit cassandraUnit = new CassandraCQLUnit(new ClassPathCQLDataSet("cassandra.cql", false, false));

}