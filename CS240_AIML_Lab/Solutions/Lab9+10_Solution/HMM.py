import json
import nltk
import numpy as np
from tqdm import tqdm
from nltk.corpus import brown
from sklearn import metrics
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.model_selection import KFold
import copy
import os


# Function to split the dataset into 5 folds for cross-validation
def get_folds(tagged_sentences_all):
    kf = KFold(n_splits=5, shuffle=True, random_state=42)
    return kf.split(tagged_sentences_all)


# Function to get the unique set of POS tags from the dataset
def get_unique_tags(tagged_sentences_all):
    tags = []
    for sentence in tagged_sentences_all:
        for _, tag in sentence:
            tags.append(tag)
    return list(set(tags))


# Function to preprocess sentences by adding start and end tags ("#####", "$$$$$")
def get_preprocessed_sentences(tagged_sentences_train):
    for i, sentence in enumerate(tagged_sentences_train):
        if sentence[0] != ("#####", "#####"):
            sentence.insert(0, ("#####", "#####"))
        if sentence[-1] != ("$$$$$", "$$$$$"):
            sentence.append(("$$$$$", "$$$$$"))
    return tagged_sentences_train


# Function to compute the probability of a tag given the previous tag (Transition Probability)
def make_tag_given_tag(tagged_sentences_train, tags, fold=None):
    tag_given_tag = {}

    # Initialize counts for tags
    for sentence in tagged_sentences_train:
        for i in range(len(sentence) - 1):
            pos1 = sentence[i][1]
            pos2 = sentence[i + 1][1]
            if pos2 not in tag_given_tag:
                tag_given_tag[pos2] = {}
                for tag in tags:
                    if tag != "$$$$$":
                        tag_given_tag[pos2][tag] = 0
            tag_given_tag[pos2][pos1] += 1

    # Calculate probabilities using Laplace smoothing
    for tag in tags:
        if tag != "$$$$$":
            total = sum(tag_given_tag[key1][tag] for key1 in tag_given_tag)
            for key1 in tag_given_tag:
                tag_given_tag[key1][tag] = (tag_given_tag[key1][tag] + 1) / (
                    total + len(tag_given_tag)
                )

    # Save transition probabilities to JSON files
    if fold is not None:
        with open(f"data/fold{fold}/tag_given_tag.json", "w") as f:
            json.dump(tag_given_tag, f)
    else:
        with open("data/full_data/tag_given_tag.json", "w") as f:
            json.dump(tag_given_tag, f)


# Function to compute the probability of a word given a tag (Emission Probability)
def make_word_given_tag(tagged_sentences_train, tags, fold=None):
    word_given_tag = {}

    # Initialize counts for words given tags
    for sentence in tagged_sentences_train:
        for word, tag in sentence:
            word = word.lower()
            if word not in word_given_tag:
                word_given_tag[word] = {}
                for tag in tags:
                    word_given_tag[word][tag] = 0
            word_given_tag[word][tag] += 1

    total_given_tag = {}
    # Calculate probabilities using Laplace smoothing
    for tag in tags:
        total = sum(word_given_tag[word][tag] for word in word_given_tag)
        total_given_tag[tag] = total
        for word in word_given_tag:
            word_given_tag[word][tag] = (word_given_tag[word][tag] + 1) / (
                total + len(word_given_tag)
            )

    # Save emission probabilities to JSON files
    if fold is not None:
        with open(f"data/fold{fold}/word_given_tag.json", "w") as f:
            json.dump(word_given_tag, f)
        with open(f"data/fold{fold}/total_given_tag.json", "w") as f:
            json.dump(total_given_tag, f)
    else:
        with open("data/full_data/word_given_tag.json", "w") as f:
            json.dump(word_given_tag, f)
        with open("data/full_data/total_given_tag.json", "w") as f:
            json.dump(total_given_tag, f)


# Function to create a mapping of tags to indices
def make_tag_index_map(tags, fold=None):
    tags.remove("#####")
    tag_to_index = {}

    # Assign a unique index to each tag
    for tag in tags:
        if tag != "$$$$$":
            tag_to_index[tag] = len(tag_to_index)

    # Save tag index map to JSON files
    if fold is not None:
        with open(f"data/fold{fold}/tag_to_index.json", "w") as f:
            json.dump(tag_to_index, f)
    else:
        with open("data/full_data/tag_to_index.json", "w") as f:
            json.dump(tag_to_index, f)


# Function to perform POS tagging using the Viterbi algorithm
def get_POS_tags(
    sentence, tag_given_tag, word_given_tag, total_given_tag, tag_to_index
):
    primary_prob = []
    temp_prob = np.zeros((len(tag_to_index), len(tag_to_index)))

    # Initialize primary probabilities for the first word
    for tag in tag_to_index:
        primary_prob.append(([tag], tag_given_tag[tag]["#####"]))

    # Iterate over each word in the sentence except the last
    for word in sentence[:-1]:
        for tag in tag_to_index:
            for tag2 in tag_to_index:
                try:
                    temp_prob[tag_to_index[tag], tag_to_index[tag2]] = (
                        tag_given_tag[tag2][tag]
                        * word_given_tag[word][tag]
                        * primary_prob[tag_to_index[tag]][1]
                    )
                except: # Handle unseen words (Laplace smoothing)
                    temp_prob[tag_to_index[tag], tag_to_index[tag2]] = (
                        tag_given_tag[tag2][tag]
                        * 1
                        / (total_given_tag[tag] + len(word_given_tag))
                        * primary_prob[tag_to_index[tag]][1]
                    )

        # Update primary probabilities for the next word
        temp_list = []
        for tag in tag_to_index:
            max_prob = np.max(temp_prob[:, tag_to_index[tag]])
            max_index = np.argmax(temp_prob[:, tag_to_index[tag]])
            temp_list.append((primary_prob[max_index][0] + [tag], max_prob))
        primary_prob = temp_list

    # Handle the last word of the sentence
    for i, elem in enumerate(primary_prob):
        prob = elem[1]
        try:
            new_prob = (
                prob
                * tag_given_tag["$$$$$"][elem[0][-1]]
                * word_given_tag[sentence[-1]][elem[0][-1]]
            )
        except:
            new_prob = (
                prob
                * tag_given_tag["$$$$$"][elem[0][-1]]
                * 1
                / (total_given_tag[elem[0][-1]] + len(word_given_tag))
            )
        primary_prob[i] = (elem[0], new_prob)

    # Select the sequence with the maximum probability
    max = -1
    for elem in primary_prob:
        if elem[1] > max:
            max = elem[1]
            max_elem = elem[0]

    return max_elem


# Function to compute the confusion matrix for evaluation
def confmat(predicted, actual, tag_to_index):
    confusion_matrix = metrics.confusion_matrix(
        actual, predicted, labels=list(tag_to_index.keys())
    )
    return confusion_matrix


# Function to train the model on the full dataset
def full_train(tagged_sentences_all, tags):
    tagged_sentences_all = copy.deepcopy(tagged_sentences_all)
    tags = copy.deepcopy(tags)

    # Preprocess sentences and compute transition and emission probabilities
    tagged_sentences_all = get_preprocessed_sentences(tagged_sentences_all)
    make_tag_given_tag(tagged_sentences_all, tags)
    make_word_given_tag(tagged_sentences_all, tags)
    make_tag_index_map(tags)


# Main function to execute the program
def main():
    # Download necessary NLTK resources
    nltk.download("brown")
    nltk.download("universal_tagset")

    # Load Brown corpus and extract sentences with universal tags
    tagged_sentences_all = brown.tagged_sents(tagset="universal")
    tagged_sentences_all = list(tagged_sentences_all)

    # Get unique POS tags and add start and end markers
    tags_org = get_unique_tags(tagged_sentences_all)
    tags_org.append("$$$$$")
    tags_org.append("#####")

    predicted_all = []
    actual_all = []

    # Create directory for storing results if it doesn't exist
    if not os.path.exists(f"data/full_data"):
        os.makedirs(f"data/full_data")

    # Perform cross-validation with 5 folds
    folds = get_folds(tagged_sentences_all)
    for fold, (train_index, test_index) in enumerate(folds, 1):
        print(f"Fold {fold}")
        tagged_sentences_train = [tagged_sentences_all[i] for i in train_index]
        tagged_sentences_test = [tagged_sentences_all[i] for i in test_index]

        # Deep copy the sentences and tags for manipulation
        tagged_sentences_train = copy.deepcopy(tagged_sentences_train)
        tagged_sentences_test = copy.deepcopy(tagged_sentences_test)
        tags = copy.deepcopy(tags_org)

        # Create directory for storing fold results if it doesn't exist
        if not os.path.exists(f"data/fold{fold}"):
            os.makedirs(f"data/fold{fold}")

        # Preprocess sentences and compute transition and emission probabilities for each fold
        tagged_sentences_train = get_preprocessed_sentences(tagged_sentences_train)
        make_tag_given_tag(tagged_sentences_train, tags, fold)
        make_word_given_tag(tagged_sentences_train, tags, fold)
        make_tag_index_map(tags, fold)

        predicted = []
        actual = []

        # Load computed probabilities for POS tagging
        with open(f"data/fold{fold}/tag_given_tag.json", "r") as f:
            tag_given_tag = json.load(f)

        with open(f"data/fold{fold}/word_given_tag.json", "r") as f:
            word_given_tag = json.load(f)

        with open(f"data/fold{fold}/total_given_tag.json", "r") as f:
            total_given_tag = json.load(f)

        with open(f"data/fold{fold}/tag_to_index.json", "r") as f:
            tag_to_index = json.load(f)

        # Perform POS tagging on test sentences
        for sentence in tqdm(tagged_sentences_test):
            actual.extend([tag for _, tag in sentence])
            predicted.extend(
                get_POS_tags(
                    [word.lower() for word, _ in sentence],
                    tag_given_tag,
                    word_given_tag,
                    total_given_tag,
                    tag_to_index,
                )
            )

        predicted_all.extend(predicted)
        actual_all.extend(actual)

    # Compute confusion matrix and save it as a heatmap
    confusion_matrix = confmat(predicted_all, actual_all, tag_to_index)
    plt.figure(figsize=(12, 8))
    sns.heatmap(
        confusion_matrix,
        annot=True,
        cmap="Reds",
        fmt="d",
        xticklabels=list(tag_to_index.keys()),
        yticklabels=list(tag_to_index.keys()),
    )
    plt.xlabel("Predicted")
    plt.ylabel("Actual")
    plt.title("Confusion Matrix")
    plt.savefig("confusion_matrix.png", dpi=300, bbox_inches="tight")

    # Print classification report and performance metrics for different beta values
    print(
        metrics.classification_report(
            actual_all, predicted_all, labels=list(tag_to_index.keys()), digits=4
        )
    )
    print(
        metrics.precision_recall_fscore_support(
            actual_all,
            predicted_all,
            average="weighted",
            labels=list(tag_to_index.keys()),
            beta=1,
        )
    )
    print(
        metrics.precision_recall_fscore_support(
            actual_all,
            predicted_all,
            average="weighted",
            labels=list(tag_to_index.keys()),
            beta=0.5,
        )
    )
    print(
        metrics.precision_recall_fscore_support(
            actual_all,
            predicted_all,
            average="weighted",
            labels=list(tag_to_index.keys()),
            beta=2,
        )
    )

    # Train on the full dataset
    full_train(tagged_sentences_all, tags_org)


if __name__ == "__main__":
    main()
