import React, { Component } from 'react';
import { View, Text, StyleSheet } from 'react-native';
import { Actions } from 'react-native-router-flux';

export default class MyBeer extends Component {
  render() {
    return (
      <View style={{margin: 128}}>
        <Text onPress={Actions.pageTwo}>This is Page Two!</Text>
      </View>
    )
  }
}