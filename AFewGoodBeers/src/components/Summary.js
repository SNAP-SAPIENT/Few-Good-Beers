import React, { Component } from 'react';
import { View, Text, StyleSheet } from 'react-native';
import { Actions } from 'react-native-router-flux';

import textStyle from '../styles/textStyle';

export default class Summary extends Component {
  render() {
    return (
      <View>
        <View>
          <Text style={textStyle.header} onPress={Actions.profile}>Summary Component</Text>
        </View>
      </View>
    )
  }
}