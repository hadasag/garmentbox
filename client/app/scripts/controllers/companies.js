'use strict';

/**
 * @ngdoc function
 * @name clientApp.controller:CompaniesCtrl
 * @description
 * # CompaniesCtrl
 * Controller of the clientApp
 */
angular.module('clientApp')
  .controller('CompaniesCtrl', function ($scope, Companies, $state, $log) {

    // Initialize values.
    $scope.companies = null;
    $scope.selectedCompany = null;

    $scope.$on('$stateChangeSuccess', function(event, toState, toParams){
      if (!$state.includes('dashboard.companies')) {
        return;
      }

      // Load companies.
      Companies.get().then(function(companies) {
        $scope.companies = companies;

        if (toParams.id) {
          setSelectedCompany(toParams.id);
        }
      });
    });

    /**
     * Set the selected Company.
     *
     * @param int id
     *   The company ID.
     */
    var setSelectedCompany = function(id) {
      $scope.selectedCompany = null;

      angular.forEach($scope.companies, function(value) {
        if (value.id == id) {
          $scope.selectedCompany = value;
        }
      });
    };

  });