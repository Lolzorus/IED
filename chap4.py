# *******************************************************
# Nom ......... : Version complete
# Role ........ : App streamlit chapitre 4
# Auteur ...... : Damien, HAPPEY
# Version ..... : V1 du 25Juillet 2023
# Licence ..... : realise dans le cadre du cours Outils collaboratifs
# (../..)
# Compilation : python3
# Usage : Pour executer : streamlit run chap4.py
#********************************************************/

import streamlit as st
from PIL import Image
from geopy.geocoders import Nominatim
import folium
from streamlit_folium import folium_static
from unidecode import unidecode  # Import unidecode library

# Les 3 tags EXIF les plus importants (On peut rajouter suivant les besoins)
TAGS_EXIF_IMPORTANTS = ["Modele", "Fabricant", "DateHeureOriginale"]

# TAG GPS
TAGS_GPS_IMPORTANTS = ["LatitudeGPS", "LongitudeGPS"]

def afficher_metadonnees_exif(image):
    metadonnees_exif = image.info.get('exif')
    if metadonnees_exif is not None:
        # Convertion bytes au cas ou (gere certains messages d'erreur suivant les images)
        if isinstance(metadonnees_exif, bytes):
            metadonnees_exif = metadonnees_exif.decode('utf-8')

        st.subheader("Metadonnees EXIF :")
        for tag in TAGS_EXIF_IMPORTANTS:
            # Stocke les EXIF
            if isinstance(metadonnees_exif, str):
                import json
                metadonnees_exif = json.loads(metadonnees_exif)

            valeur = metadonnees_exif.get(tag, "")
            st.write(f"{tag}: {valeur}")

def afficher_metadonnees_gps(image):
    metadonnees_exif = image.info.get('exif')
    if metadonnees_exif is not None:
        # Conversion bytes si necessaire
        if isinstance(metadonnees_exif, bytes):
            metadonnees_exif = metadonnees_exif.decode('utf-8')

        st.subheader("Donnees GPS :")
        for tag in TAGS_GPS_IMPORTANTS:
            if isinstance(metadonnees_exif, str):
                import json
                metadonnees_exif = json.loads(metadonnees_exif)

            valeur = metadonnees_exif.get(tag, "")
            st.write(f"{tag}: {valeur}")

def editer_metadonnees_exif(image, metadonnees):
    metadonnees_exif = image.info.get('exif')
    if metadonnees_exif is not None:
        metadonnees_exif.update(metadonnees)
    else:
        image.info['exif'] = metadonnees

def editer_metadonnees_gps(image, latitude, longitude):
    metadonnees_exif = image.info.get('exif')
    if metadonnees_exif is not None:
        # Convertir les bytes en dictionnaire si necessaire
        if isinstance(metadonnees_exif, bytes):
            metadonnees_exif = metadonnees_exif.decode('utf-8')

        # Charger les donnees EXIF dans un dictionnaire (s'il s'agit d'une representation de chaine)
        if isinstance(metadonnees_exif, str):
            import json
            metadonnees_exif = json.loads(metadonnees_exif)

        # Mettre a jour les tags GPS avec les valeurs de latitude et longitude fournies
        metadonnees_exif["LatitudeGPS"] = latitude
        metadonnees_exif["LongitudeGPS"] = longitude

        # Encoder le dictionnaire en bytes
        metadonnees_bytes = json.dumps(metadonnees_exif).encode('utf-8')
        image.info['exif'] = metadonnees_bytes

def obtenir_coordonnees_gps_actuelles():
    # Initialiser le geolocalisateur avec un agent utilisateur valide
    geolocator = Nominatim(user_agent="geoapiExercises")

    # Obtenir l'adresse de l'emplacement actuel
    emplacement_actuel = geolocator.geocode("")

    if emplacement_actuel:
        # Extraire la latitude et la longitude de l'emplacement
        latitude = str(emplacement_actuel.latitude)
        longitude = str(emplacement_actuel.longitude)
        return latitude, longitude
    else:
        st.warning("Impossible de recuperer les coordonnees GPS.")
        return None, None

def afficher_carte(latitude, longitude, pois):
    # Créer une carte folium centrée sur les coordonnées de latitude et longitude fournies
    ma_carte = folium.Map(location=[latitude, longitude], zoom_start=15)

    # Ajouter un marqueur pour chaque POI
    for poi in pois:
        folium.Marker([poi['latitude'], poi['longitude']], popup=poi['name']).add_to(ma_carte)

    # Afficher la carte en utilisant streamlit
    folium_static(ma_carte)

def principal():
    st.title("Editeur de Metadonnees EXIF")

    image_chargee = st.file_uploader("Choisissez une image (jpg)", type=["jpg", "jpeg"])

    # initialization des latitudes et longitudes avec None
    latitude_input = None
    longitude_input = None

    if image_chargee is not None:
        image = Image.open(image_chargee)

        # Affichage des metadonnees EXIF immediatement apres le chargement de l'image
        afficher_metadonnees_exif(image)

        # Affichage des metadonnees GPS
        afficher_metadonnees_gps(image)

        st.image(image, caption="Image originale", use_column_width=True)

        # Formulaire pour modifier les metadonnees
        st.subheader("Modifier les Metadonnees EXIF :")
        metadonnees = {}
        for tag in TAGS_EXIF_IMPORTANTS:
            valeur = st.text_input(tag, key=tag, value="")
            metadonnees[tag] = valeur

        if st.button("Enregistrer les modifications"):
            editer_metadonnees_exif(image, metadonnees)
            st.success("Metadonnees modifiees avec succes !")
            # Afficher les metadonnees mises a jour apres l'enregistrement
            afficher_metadonnees_exif(image)
            # Afficher l'image modifiee
            st.image(image, caption="Image modifiee", use_column_width=True)

        # Formulaire pour modifier les donnees GPS
        st.subheader("Modifier les Donnees GPS :")
        latitude_input = st.text_input("Latitude", key="LatitudeGPS", value="")
        longitude_input = st.text_input("Longitude", key="LongitudeGPS", value="")

        if st.button("Enregistrer les modifications GPS"):
            editer_metadonnees_gps(image, latitude_input, longitude_input)
            st.success("Donnees GPS modifiees avec succes !")
            # Afficher les donnees GPS mises a jour apres l'enregistrement
            afficher_metadonnees_gps(image)
            # Afficher l'image modifiee
            st.image(image, caption="Image modifiee", use_column_width=True)

    # Liste de POI en italie
    pois_perso = [
        {"name": "Colisee", "latitude": 41.8902, "longitude": 12.4922},
        {"name": "Tour de pise", "latitude": 43.7230, "longitude": 10.3966},
        {"name": "Venise", "latitude": 45.4408, "longitude": 12.3155},
        {"name": "Monaco", "latitude": 43.7383, "longitude": 7.4244},
        {"name": "Geneve", "latitude": 46.2043, "longitude": 6.1431}
        # On peut rajouter des POI ici
    ]

    # affiche les POI sur la carte
    if latitude_input and longitude_input:
        latitude = float(latitude_input)
        longitude = float(longitude_input)
        st.subheader("Position sur la carte :")
        afficher_carte(latitude, longitude, pois_perso)

if __name__ == "__main__":
    principal()


