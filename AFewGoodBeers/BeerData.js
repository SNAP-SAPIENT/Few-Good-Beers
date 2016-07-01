import React, { Component } from 'react';

import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

var noble = require('noble');

export default class BeerData extends Component {
  constructor(){
    super();
    this.state = {
      testData: 10
    }
  }

  componentWillMount(){
    noble.on('stateChange', this._onStateChange);
    noble.on('discover', this._onPeripheralFound);
  }

  componentWillUnMount(){
    noble.stopScanning();
  }

  render() {
    return(
      <View>
        <Text>{this.state.testData}</Text>
      </View>
    )
  }

  _onStateChange(state) {
      if (state === 'poweredOn') {
        console.log('start the scan');
        noble.startScanning(['103D']);
      } else {
        console.log('not scanning');
        noble.stopScanning();
      }
  }

  _onPeripheralFound = (peripheral) => {
    console.log('found it?');
    this._printPeripheral(peripheral);
    this._connectToBeers(peripheral);
  }

  _connectToBeers(peripheral) {
    console.log('connect, yo');
    peripheral.connect(discover);
  }

  _printPeripheral(peripheral) {
      console.log('peripheral discovered (' + peripheral.id +
                  ' with address <' + peripheral.address +  ', ' + peripheral.addressType + '>,' +
                  ' connectable ' + peripheral.connectable + ',' +
                  ' RSSI ' + peripheral.rssi + ':');
      console.log('\thello my local name is:');
      console.log('\t\t' + peripheral.advertisement.localName);
      console.log('\tcan I interest you in any of the following advertised services:');
      console.log('\t\t' + JSON.stringify(peripheral.advertisement.serviceUuids));

      var serviceData = peripheral.advertisement.serviceData;
      console.log('peripheral adv. ', peripheral.advertisement);
      console.log('service Data: ', serviceData);
      // if (serviceData && serviceData.length) {
      //   console.log('\there is my service data:');
      //   for (var i in serviceData) {
      //     console.log('\t\t' + JSON.stringify(serviceData[i].uuid) + ': ' + JSON.stringify(serviceData[i].data.toString('hex')));
      //   }
      // }
      if (peripheral.advertisement.manufacturerData) {
        console.log('\there is my manufacturer data:');
        console.log('\t\t' + JSON.stringify(peripheral.advertisement.manufacturerData.toString('hex')));
      }
      if (peripheral.advertisement.txPowerLevel !== undefined) {
        console.log('\tmy TX power level is:');
        console.log('\t\t' + peripheral.advertisement.txPowerLevel);
      }
    }
}

//<Text> Peripheral address: peripheral.address </Text>
  //        <Text> Peripheral address type: peripheral.addressType </Text>
    //      <Text> Peripheral connectable: {peripheral.connectable}</Text>
