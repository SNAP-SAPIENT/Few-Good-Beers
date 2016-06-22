import React, { Component } from 'react';
import { View, Text, StyleSheet } from 'react-native';
import { Actions } from 'react-native-router-flux';

import textStyle from '../../styles/textStyle';

export default class Profile extends Component {
  render() {
    return (
      <View style={styles.container}>
        <View>
          <Text style={textStyle.header}>Profile View</Text>
        </View>
      </View>
    )
  }
}

var styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'space-around',
        alignItems: 'center',
        backgroundColor: 'transparent',
    }
});
