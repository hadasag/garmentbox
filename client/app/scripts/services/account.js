'use strict';

/**
 * @ngdoc service
 * @name clientApp.account
 * @description
 * # account
 * Service in the clientApp.
 */
angular.module('clientApp')
  .service('Account', function ($q, $http, $timeout, Config, $rootScope, $log) {

    // A private cache key.
    var cache = {};

    /**
     * Return the promise with the items list, from cache or the server.
     *
     * @returns {*}
     */
    this.get = function() {
      return $q.when(cache.data || getDataFromBackend());
    };

    /**
     * Return items array from the server.
     *
     * @returns {$q.promise}
     */
    var getDataFromBackend = function() {
      var deferred = $q.defer();
      var url = Config.backend + '/api/me/';

      $http({
        method: 'GET',
        url: url
      }).success(function(response) {
        setCache(response.data[0]);
        deferred.resolve(response.data[0]);
      });

      return deferred.promise;
    }

    /**
     * Save data in cache, and broadcast en event to inform that the meters data changed.
     *
     * @param data
     *   The data to cache.
     */
    var setCache = function(data) {
      // Cache data.
      cache = {
        data: data,
        timestamp: new Date()
      };

      // Clear cache in 60 seconds.
      $timeout(function() {
        cache = null;
      }, 60000);

      // Broadcast a change event.
      $rootScope.$broadcast('gb.account.changed');
    };

    $rootScope.$on('clearCache', function() {
      cache = null;
    });

  });
