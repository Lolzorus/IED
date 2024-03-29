import streamlit as st
import piexif
from PIL import Image
import io
import folium
from streamlit_folium import folium_static
from geopy.distance import geodesic

st.title("Editeur Metadata EXIF et GPS")

uploaded_file = st.file_uploader("Choisissez un fichier jpg", type="jpg")

# Cette fonction permettra d'afficher et de gerer les metadonnees EXIF
def handle_metadata(section_name, data_dict):
    if section_name in data_dict:
        st.subheader(f"{section_name} Metadata")

        # Parcours les cles existantes et affiche les valeurs correspondantes
        for tag, value in data_dict[section_name].items():
            tag_info = piexif.TAGS[section_name].get(tag, {})
            tag_name = tag_info.get("name", "Unknown")
            tag_type = tag_info.get("type", "Unknown")

            # Verifie si la valeur est de type bytes (chaine encodee) ou int (nombre)
            if isinstance(value, bytes):
                new_value = st.text_input(f"{tag_name} (Type: {tag_type})", value.decode('utf-8', 'ignore'))
            else:
                new_value = st.text_input(f"{tag_name} (Type: {tag_type})", str(value))

            # Enregistre les modifications si l'utilisateur a entre une nouvelle valeur
            if new_value != str(value):
                if isinstance(value, int):
                    try:
                        data_dict[section_name][tag] = int(new_value)
                    except ValueError:
                        st.warning(f"Erreur de valeur pour {tag_name}")
                elif isinstance(value, bytes):
                    data_dict[section_name][tag] = new_value.encode('utf-8')
                else:
                    data_dict[section_name][tag] = new_value

                # tentative debugage type 5 ou 10 (ne fonctionne pas) Si la valeur est de type 5 ou 10, convertir en int avant de l'enregistrer
                if tag_type == 5 or tag_type == 10:
                    data_dict[section_name][tag] = (int(data_dict[section_name][tag][0]),)  # Convertir le tuple en tuple avec un seul element (ne fonctionne pas)


# Coordonnees GPS des villes pour le menu deroulant
CITIES_GPS = {
    "Paris": (48.8566, 2.3522),
    "Rome": (41.9028, 12.4964),
    "Seoul": (37.5665, 126.9780),
    "New-York": (40.7128, -74.0060),
    "Dubai": (25.276987, 55.296249)
}

# Convertit un nombre decimal en degres/minutes/secondes pour l'EXIF
def decimal_to_dms(decimal):
    degrees = int(decimal)
    submin = abs(decimal - degrees) * 60
    minutes = int(submin)
    subsec = abs(submin - minutes) * 60
    seconds = int(subsec * 10000)  # pour avoir une precision plus grande

    return ((degrees, 1), (minutes, 1), (seconds, 10000))


def handle_gps_metadata():
    if "GPS" in exif_dict:
        st.subheader("GPS Metadata")
        selected_city = st.selectbox("Choisissez une ville pour modifier les coordonnees GPS", list(CITIES_GPS.keys()))

        # Convertit les coordonnees de la ville selectionnee en format EXIF
        lat, lon = CITIES_GPS[selected_city]
        exif_lat = decimal_to_dms(lat)
        exif_lon = decimal_to_dms(lon)

        # Met a jour les coordonnees GPS dans le dictionnaire
        exif_dict["GPS"][piexif.GPSIFD.GPSLatitude] = exif_lat
        exif_dict["GPS"][piexif.GPSIFD.GPSLongitude] = exif_lon
        exif_dict["GPS"][piexif.GPSIFD.GPSLatitudeRef] = b'N' if lat >= 0 else b'S'
        exif_dict["GPS"][piexif.GPSIFD.GPSLongitudeRef] = b'E' if lon >= 0 else b'W'

# Convertit les coordonnees GPS au format EXIF (degres/minutes/secondes) en format decimal.
def dms_to_decimal(dms_tuple):
    degrees, minutes, seconds = dms_tuple
    decimal_deg = degrees[0] / degrees[1] + minutes[0] / minutes[1] / 60 + seconds[0] / seconds[1] / 3600
    return decimal_deg

# Recupere les coordonnees GPS d'un dictionnaire EXIF et les renvoie au format decimal.
def get_gps_coordinates(exif_data):
    lat_data = exif_data["GPS"].get(piexif.GPSIFD.GPSLatitude)

    # verifie que les coordonnees soient valides / existent ou non
    lat_ref_data = exif_data["GPS"].get(piexif.GPSIFD.GPSLatitudeRef)
    lat_ref = lat_ref_data.decode('utf-8') if lat_ref_data is not None else None

    lon_data = exif_data["GPS"].get(piexif.GPSIFD.GPSLongitude)

    # verifie que les coordonnees soient valides / existent ou non
    lon_ref_data = exif_data["GPS"].get(piexif.GPSIFD.GPSLongitudeRef)
    lon_ref = lon_ref_data.decode('utf-8') if lon_ref_data is not None else None

    # Verification pour location = None (evite les erreurs si la photo n'a pas de coordonnees GPS)
    if not lat_data or not lon_data or lat_ref is None or lon_ref is None:
        return None, None

    lat = dms_to_decimal(lat_data)
    if lat_ref == "S":
        lat = -lat

    lon = dms_to_decimal(lon_data)
    if lon_ref == "W":
        lon = -lon

    return lat, lon


#Liste des POIS (peut etre fait a partir d'une liste ou fichier CSV)
POIS = [
    {"name": "Tour eiffel", "coords": (48.858370, 2.294481)},
    {"name": "Versaille", "coords": (48.808193, 2.12667)},
    {"name": "Arc de Triomphe", "coords": (48.873792, 2.295028)},
    {"name": "Tour de Pise", "coords": (43.724591, 10.382981)},
    {"name": "Colisee", "coords": (41.890210, 12.492231)},
    {"name": "Fontaine de Trevi", "coords": (41.900883, 12.48334)},
    {"name": "N Seoul Tower", "coords": (37.551388, 126.988106)},
    {"name": "Gangnam", "coords": (37.497942, 127.027621)},
    {"name": "Gyeongbokgung Palace", "coords": (37.580467, 126.976944)},
    {"name": "Central Park", "coords": (40.778848, -73.968898)},
    {"name": "Empire state Building", "coords": (40.748441, -73.985664)},
    {"name": "Manhattan", "coords": (40.779897, -73.968565)},
    {"name": "Burj Khalifa", "coords": (25.197195, 55.274376)},
    # ... Ajoutez d'autres POI ici si besoin...
]

# Renvoie tous les points d'interet (Redondant mais on peut modifier les 2 prochaines fonctions pour choisir combien de POIs seront relies, si il y a une distance max etc etc)
def closest_pois(latitude, longitude, pois):
    return pois

# Lie tous les points d'interet avec des lignes sur la carte m
def link_pois_with_lines(m, pois):
    # Recuperer les coordonnees de tous les POIs
    coords = [poi["coords"] for poi in pois]

    # Si on veut fermer la boucle (relier le dernier POI au premier)
    # coords.append(coords[0])

    # format ligne reliant tous les points d'interet
    folium.PolyLine(coords, color="blue", weight=2.5, opacity=0.7).add_to(m)


# Travail sur la photo upload dans l'application
if uploaded_file:
    st.image(uploaded_file, caption="Image chargee", use_column_width=True)

    # Lire les donnees EXIF de l'image
    exif_dict = piexif.load(uploaded_file.getvalue())

    # Afficher et gerer les metadonnees EXIF pour la section Exif
    handle_metadata("Exif", exif_dict)

    # Recuperation des coordonnees GPS originales et affichage sur une carte
    latitude, longitude = get_gps_coordinates(exif_dict)
    if latitude and longitude:
        st.map([{"lat": latitude, "lon": longitude}], zoom=10)

        # Affiche sur une carte folium
        m = folium.Map(location=[latitude, longitude], zoom_start=10)
        folium.Marker([latitude, longitude], tooltip="Original Location", icon=folium.Icon(color="red")).add_to(m)

        nearest_pois = closest_pois(latitude, longitude, POIS)
        for poi in nearest_pois:
            folium.Marker(poi["coords"], tooltip=poi["name"]).add_to(m)

        link_pois_with_lines(m, nearest_pois)  # relie les POIs avec une ligne

        folium_static(m)

    # Gere les coordonnees GPS a l'aide du menu deroulant
    handle_gps_metadata()

    # Bouton pour enregistrer les modifications
    if st.button("Enregistrer changements"):
        exif_bytes = piexif.dump(exif_dict)
        im = Image.open(uploaded_file)
        output = io.BytesIO()
        im.save(output, format="JPEG", exif=exif_bytes)

        st.download_button(
            label="Telecharger photo modifiee",
            data=output.getvalue(),
            file_name="image_editee.jpg",
            mime="image/jpeg"
        )