import streamlit as st
from HMM import get_POS_tags  # Assuming you have a function that returns POS tags
import json
import pandas as pd

# Load the transition probabilities
with open("data/full_data/tag_given_tag.json") as f:
    tag_given_tag = json.load(f)

# Load the emission probabilities
with open("data/full_data/word_given_tag.json") as f:
    word_given_tag = json.load(f)

# Load the total count of each tag
with open("data/full_data/total_given_tag.json") as f:
    total_given_tag = json.load(f)

# Load the tag to index mapping
with open("data/full_data/tag_to_index.json") as f:
    tag_to_index = json.load(f)

# Title for the app
st.title("Part-of-Speech Tagging with HMM")

# Example inputs
good_inputs = [
    "I want to eat french fries .",
    "I have ten thousand candies and chocolates .",
    "I told you to stop being a crybaby .",
    "I want to run .",
    "I want to go for a run .",
]

bad_inputs = [
    "Neel , Eshaan , Deeptanshu and Deevyanshu are in a team .",
    "I am working to people remote areas .",
    "We will go to our room and do masti .",
]

# Input text box
st.write("Enter a sentence to get its POS tags:")
input_text = st.text_input(placeholder="Type here...", label="Input sentence")

st.write("Or choose from the following examples:")

col1, col2 = st.columns(2)
# Display example input buttons for user convenience
with col1:
    with st.container(border=True):
        st.write("Good examples:")
        for example in good_inputs:
            if st.button(example):
                input_text = example

with col2:
    with st.container(border=True):
        st.write("Bad examples:")
        for example in bad_inputs:
            if st.button(example):
                input_text = example

# Button to trigger POS tagging
if input_text:
    # Get the POS tags using your HMM model
    pos_tags = get_POS_tags(
        input_text.split(), tag_given_tag, word_given_tag, total_given_tag, tag_to_index
    )

    st.divider()

    # Display the result as a dictionary
    st.write("POS Tags for the input sentence:")

    df = pd.DataFrame({"Word": input_text.split(), "POS Tag": pos_tags})
    df.reset_index(drop=True, inplace=True)
    st.table(df)
